import numpy as np
import cv2
 
img1 = cv2.imread('C:/homework/imageset/small1.jpg',0)
img2 = cv2.imread('C:/homework/imageset/big1.jpg',0)

#img1 = cv2.resize(img1,(300,300))
#img2 = cv2.resize(img2,(300,300))

sift = cv2.xfeatures2d.SIFT_create()
 
kp1, des1 = sift.detectAndCompute(img1,None)
kp2, des2 = sift.detectAndCompute(img2,None)
 
bf = cv2.BFMatcher()
matches = bf.knnMatch(des1,des2, k=2)
 
good = []
for m,n in matches:
    if m.distance < 0.3*n.distance:
        good.append([m])
 
img3 = cv2.drawMatchesKnn(img1,kp1,img2,kp2,good,None,flags=2)

cv2.imshow('result', img3)
cv2.waitKey(0)
cv2.destroyAllWindows()