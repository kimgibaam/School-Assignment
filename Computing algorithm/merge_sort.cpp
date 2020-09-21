#include<stdio.h>

struct things {               //물건을 담을 구조체
	char name[10];  // 이름
	int weight;      // 무게
	int volume;      // 부피
};

void merge(int low, int mid, int high);   // 합병정렬 함수
void mergeSort(int low, int high);         // 분할 함수
void copyArray(int start, int end);        // 정렬후 내용 복사

struct things thing[16];         // 물건내용 담을 구조체
struct things temp[16];           // 정렬을 위한 임시 구조체


void mergeSort(int low, int high) {
	int mid;
	if (low < high) {
		mid = (low + high) / 2;       // 중간지점을 정하여 배열을 두 구간으로 분할 반복
		mergeSort(low, mid);
		mergeSort(mid + 1, high);
		merge(low, mid, high);            // 배열이 하나가 남을때까지 분열 후 병합정렬 실시
	}
}

void merge(int low, int mid, int high) {     // 병합정렬

	int i = low;
	int j = mid + 1;
	int k = low;

	while (i <= mid && j <= high) {
		if (thing[i].weight < thing[j].weight) {         // 무게로 비교 후
			temp[k++] = thing[i++];                      // 작은 값을 먼저 임시배열에 넣음
		}
		else if (thing[i].weight >= thing[j].weight) {  // 마찬가지로 무게 비교 후
			temp[k++] = thing[j++];                      // 작은 무게를 가진 구조체를 먼저 넣음
		}
	}

	// 모든 값을 비교한 후 한쪽에만 값이 남아있는 경우 조사후 복사

	if (i >= mid) {
		while (j <= high) {
			temp[k++] = thing[j++];
		}
	}

	if (j >= high) {
		while (i <= mid) {
			temp[k++] = thing[i++];
		}
	}
	copyArray(low, high);     // 임시 구조체에 정렬해둔 값을 원래의 배열에 단순 복사
}

void copyArray(int start, int end) {     
	int i;
	for (i = start; i <= end; i++) {
		thing[i] = temp[i];
	}
}


int main()
{

	FILE *fp = fopen("input.txt", "r");    //읽기 모드로 입력파일 오픈

	for (int i = 0; i < 16; i++)
	{
		fscanf(fp, "%s %d %d ", &thing[i].name, &thing[i].weight, &thing[i].volume);  //파일 내용 읽기
		printf("%s %d %d\n", thing[i].name, thing[i].weight, thing[i].volume);     //화면에 출력
	}
	fclose(fp);

	mergeSort(0, 15);     // 무게를 기준으로 병합정렬 실행

	printf("\n무게를 기준으로 정렬 후\n\n");
	fopen("output.txt", "w");     //쓰기 모드로 출력파일 오픈

	for (int i = 0; i < 16; i++)
	{
		fprintf(fp, "%s %d %d ", thing[i].name, thing[i].weight, thing[i].volume);   //파일에 정렬된 내용 쓰기
		printf("%s %d %d\n", thing[i].name, thing[i].weight, thing[i].volume);  // 화면에 출력
	}


	return 0;
}