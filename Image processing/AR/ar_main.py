
import argparse

import cv2
import numpy as np
import math
import os
from objloader_simple import *

# 찾아야 하는 최소 일치 항목 수
# 인식 유효성을 고려
MIN_MATCHES = 220


def main():

    homography = None 
    # 카메라 파라미터의 매트릭스
    camera_parameters = np.array([[800, 0, 320], [0, 800, 240], [0, 0, 1]])
    # OBR 키포인트 검출기 생성
    orb = cv2.ORB_create()
    # 해밍 거리를 기준으로 BFMatcher 객체 생성
    bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)
    # 비디오 스트림에서 검색할 기준 표면 로드
    dir_name = os.getcwd()
    model = cv2.imread(os.path.join(dir_name, 'reference/card_s.jpg'),0)
    # 모델 키포인트 및 설명자 계산
    kp_model, des_model = orb.detectAndCompute(model, None)
    # OBJ 파일에서 3D 모델 로드
    obj = OBJ(os.path.join(dir_name, 'models/fox.obj'), swapyz=True)  
    obj2 = OBJ(os.path.join(dir_name, 'models/utah-teapot.obj'), swapyz=True)  
    # 비디오 캡처 초기화
    cap = cv2.VideoCapture(0)

    while True:
       # 현재 프레임 읽기
        ret, frame = cap.read()
        if not ret:
            print "Unable to capture video"
            return 
         # 프레임의 키포인트를 찾아서 그립니다.
        kp_frame, des_frame = orb.detectAndCompute(frame, None)
        # 모델 디스크립터와 프레임 디스크립터 일치
        matches = bf.match(des_model, des_frame)
         # 거리 순서대로 정렬
        # 거리가 멀수록 일치하는 것이 좋습니다
        matches = sorted(matches, key=lambda x: x.distance)

       # 일치하는 항목이 충분하면 호모 그래피 계산
        if len(matches) > MIN_MATCHES:
            # 소스 포인트와 대상 포인트의 차이
            src_pts = np.float32([kp_model[m.queryIdx].pt for m in matches]).reshape(-1, 1, 2)
            dst_pts = np.float32([kp_frame[m.trainIdx].pt for m in matches]).reshape(-1, 1, 2)
          # 호모 그래피 계산
            homography, mask = cv2.findHomography(src_pts, dst_pts, cv2.RANSAC, 3.0)
            if args.rectangle:
               # 프레임에서 발견 된 모델을 표시하는 사각형을 그립니다.
                h, w = model.shape
                pts = np.float32([[0, 0], [0, h - 1], [w - 1, h - 1], [w - 1, 0]]).reshape(-1, 1, 2)
                # 모서리를 프레임으로 투영
                dst = cv2.perspectiveTransform(pts, homography)
                # 선으로 연결  
                frame = cv2.polylines(frame, [np.int32(dst)], True, 255, 3, cv2.LINE_AA)  
            # 유효한 호모 그래피 행렬이 발견되면 모델 평면에서 큐브를 렌더링
            if homography is not None:
                try:
                    # 호모 그래피 매트릭스 및 카메라 파라미터에서 3D 프로젝션 매트릭스를 얻습니다
                    projection = projection_matrix(camera_parameters, homography)  
                    # 프로젝트 큐브 또는 모델
                    frame = render(frame, obj, projection, model, False)
                    frame = render2(frame, obj2, projection, model, False)
                    #frame = 렌더링 (프레임, 모델, 투영)
                except:
                    pass
             # 처음 10번까지 그립니다.
            if args.matches:
                frame = cv2.drawMatches(model, kp_model, frame, kp_frame, matches[:10], 0, flags=2)
            # 결과물
            cv2.imshow('frame', frame)
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

        else:
            print "Not enough matches found - %d/%d" % (len(matches), MIN_MATCHES)

    cap.release()
    cv2.destroyAllWindows()
    return 0

def render(img, obj, projection, model, color=False):
    """
   로드 된 obj 모델을 현재 비디오 프레임으로 렌더링
    """
    vertices = obj.vertices
    scale_matrix = np.eye(3) * 3
    h, w = model.shape

    for face in obj.faces:
        face_vertices = face[0]
        points = np.array([vertices[vertex - 1] for vertex in face_vertices])
        points = np.dot(points, scale_matrix)
          # 참조 표면의 중간에 모델을 렌더링합니다. 그렇게하려면
        # 모델 포인트는 변위되어야합니다
        points = np.array([[p[0] + w / 2, p[1] + h / 2, p[2]] for p in points])
        dst = cv2.perspectiveTransform(points.reshape(-1, 1, 3), projection)
        imgpts = np.int32(dst)
        if color is False:
            cv2.fillConvexPoly(img, imgpts, (137, 27, 211))
        else:
            color = hex_to_rgb(face[-1])
            color = color[::-1]  # 반전
            cv2.fillConvexPoly(img, imgpts, color)

    return img

def render2(img, obj, projection, model, color=False):

    vertices = obj.vertices
    scale_matrix = np.eye(3) * 3
    h, w = model.shape

    for face in obj.faces:
        face_vertices = face[0]
        points = np.array([vertices[vertex - 1] for vertex in face_vertices])
        points = np.dot(points, scale_matrix)

        points = np.array([[p[0] + w / 2 + 200, p[1] + h / 2, p[2]] for p in points])
        dst = cv2.perspectiveTransform(points.reshape(-1, 1, 3), projection)
        imgpts = np.int32(dst)
        if color is False:
            cv2.fillConvexPoly(img, imgpts, (0, 0, 128))
        else:
            color = hex_to_rgb(face[-1])
            color = color[::-1] 
            cv2.fillConvexPoly(img, imgpts, color)

    return img


def projection_matrix(camera_parameters, homography):
    """
    카메라 보정 매트릭스 및 추정 된 호모 그래피에서
    3D 프로젝션 매트릭스를 계산
    """
       # x와 y 축을 따라 회전뿐만 아니라 변환을 계산
    homography = homography * (-1)
    rot_and_transl = np.dot(np.linalg.inv(camera_parameters), homography)
    col_1 = rot_and_transl[:, 0]
    col_2 = rot_and_transl[:, 1]
    col_3 = rot_and_transl[:, 2]
     # 벡터 정규화
    l = math.sqrt(np.linalg.norm(col_1, 2) * np.linalg.norm(col_2, 2))
    rot_1 = col_1 / l
    rot_2 = col_2 / l
    translation = col_3 / l
    # 정규 직교 기준을 계산

    c = rot_1 + rot_2
    p = np.cross(rot_1, rot_2)
    d = np.cross(c, p)
    rot_1 = np.dot(c / np.linalg.norm(c, 2) + d / np.linalg.norm(d, 2), 1 / math.sqrt(2))
    rot_2 = np.dot(c / np.linalg.norm(c, 2) - d / np.linalg.norm(d, 2), 1 / math.sqrt(2))
    rot_3 = np.cross(rot_1, rot_2)
    # 마지막으로, 모델에서 현재 프레임까지 3D 프로젝션 매트릭스를 계산
    projection = np.stack((rot_1, rot_2, rot_3, translation)).T
    return np.dot(camera_parameters, projection)

def hex_to_rgb(hex_color):
    """
    16 진 문자열을 RGB로 변환하는 도우미 기능
    """
    hex_color = hex_color.lstrip('#')
    h_len = len(hex_color)
    return tuple(int(hex_color[i:i + h_len // 3], 16) for i in range(0, h_len, h_len // 3))


# 커맨드 라인 인자 파싱
# 모두 지원되는 것은 아닙니다.
parser = argparse.ArgumentParser(description='Augmented reality application')

parser.add_argument('-r','--rectangle', help = 'draw rectangle delimiting target surface on frame', action = 'store_true')
parser.add_argument('-mk','--model_keypoints', help = 'draw model keypoints', action = 'store_true')
parser.add_argument('-fk','--frame_keypoints', help = 'draw frame keypoints', action = 'store_true')
parser.add_argument('-ma','--matches', help = 'draw matches between keypoints', action = 'store_true')
# TODO jgallostraa-> 모델 사양에 대한 지원 추가
# parser.add_argument ( '-mo', '-model', help = '투영 할 모델 지정', 작업 = 'store_true')

args = parser.parse_args()

if __name__ == '__main__':
    main()
