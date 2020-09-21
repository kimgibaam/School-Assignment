fscanf 안에 &를 주목!
////////////////////////////

#include<stdio.h>

struct things {               //물건을 담을 구조체
	char name[10];  // 이름
	int weight;      // 무게
	int volume;      // 부피
};


int main()
{
	struct things thing[10];       //물건 10개 담을 구조체 선언
	struct things temp;            //스왑할때 임시로쓸 구조체


	FILE *fp = fopen("input.txt", "r");    //읽기 모드로 입력파일 오픈

	for (int i = 0; i < 10; i++)
	{
		fscanf(fp,"%s %d %d ", &thing[i].name, &thing[i].weight, &thing[i].volume);  //파일 내용 읽기
		printf("%s %d %d ", thing[i].name, thing[i].weight, thing[i].volume);     //화면에 출력
	}
  
	fclose(fp);    

	for (int i = 0; i < 10 - 1; i++)            //무게를 중심으로 버블정렬 수행
	{
		for (int j = 0; j < 10 - 1 - i; j++)
		{
			if (thing[j].weight > thing[j + 1].weight)
			{
				temp = thing[j];
				thing[j] = thing[j + 1];
				thing[j + 1] = temp;
			}
		}
	}
	printf("\n");   
	
	fopen("output.txt", "w");     //쓰기 모드로 출력파일 오픈

	for (int i = 0; i < 10; i++)
	{
		fprintf(fp,"%s %d %d ", thing[i].name, thing[i].weight, thing[i].volume);   //파일에 정렬된 내용 쓰기
		printf("%s %d %d ", thing[i].name, thing[i].weight, thing[i].volume);  // 화면에 출력
	}


	return 0;
}