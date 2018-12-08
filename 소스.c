#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#define n_features 71
#define n_movies 10681
#define n_clusters 1000
#define n_tags 95580

typedef struct {
	char *name;
	int m_id;
	char *genre;
	int vector[n_features];//the features of each movie
	int belong; //which cluster each movie's in
}Movie;

typedef struct {
	char *contents;
	int m_id;
	int u_id;
	int time;
}Tag;

typedef struct {
	double centorid[n_features];
	int c_id;
	int n_samples;
}Cluster;

Cluster clusters[n_clusters];
Movie movie[n_movies];
Tag tag[n_tags];
void initialize_center();
void assign_label(int index);
void compute_centroid();
void iteration();


void load_movie()
{
	char line[1000];
	FILE *fp;
	fp = fopen("movies.dat", "r");
	for (int i = 0; i < n_movies; i++)
	{
		fgets(line, sizeof(line) - 1, fp);
		char seps[] = "::";
		char *ptr = strtok(line, ":\n");
		int num = atoi(ptr);
		movie[i].m_id = num;
		ptr = strtok(NULL, ":\n");
		movie[i].name = (char*)malloc(sizeof(char)*strlen(ptr));
		strcpy(movie[i].name, ptr);
		ptr = strtok(NULL, ":\n");
		movie[i].genre = (char*)malloc(sizeof(char)*strlen(ptr));
		strcpy(movie[i].genre, ptr);
	}
	fclose(fp);
}

void load_tag()
{
	char line[1000];
	FILE *fp;
	fp = fopen("tags.dat", "r");
	for (int i = 0; i < n_tags; i++)
	{
		fgets(line, sizeof(line) - 1, fp);
		char *ptr;
		ptr = strtok(line, ":");
		int num = atoi(ptr);
		tag[i].u_id = num;
		ptr = strtok(NULL, ":");
		num = atoi(ptr);
		tag[i].m_id = num;
		ptr = strtok(NULL, ":");
		tag[i].contents = (char*)malloc(sizeof(char)*strlen(ptr));
		strcpy(tag[i].contents, ptr);
		ptr = strtok(NULL, ":\n");
		num = atoi(ptr);
		tag[i].time = num;
	}
	fclose(fp);
}

void load_feature()
{
	char line[1000];
	FILE *fp;
	fp = fopen("123.csv", "r");
	int j = 0;
	
	while(fgets(line, sizeof(line) - 1, fp)!=NULL)
	{
		char *ptr;
		for (int i = 0; i < n_features; i++)
		{
			if(i==0)
				ptr = strtok(line, ",");
			else
				ptr = strtok(NULL,  ",\n");
			
			int num = atoi(ptr);
			movie[j].vector[i] = num;
		}
		j++;
	}
	fclose(fp);
}

void initialize_center()
{
	for (int i = 0; i < n_movies; i++)
	{
		movie[i].belong = rand() % n_clusters;
	}
}

void assign_label()
{
	double temp = 0;
	double distance = 0;

	for (int i = 0; i < n_movies; i++)
	{
		temp = distance;
		distance = 0;
		for (int j = 0; j < n_clusters; j++)
		{
			for (int k = 0; j < n_features; j++)
			{
				distance += pow(clusters[j].centorid[k] - (double)movie[i].vector[k], 2);
			}
			if (distance < temp)
			{
				movie[i].belong = j;
			}
		}
	}
}


void compute_centroid()
{
	for (int i = 0; i < n_clusters; i++)
	{
		clusters[i].n_samples = 0;
		double temp[n_features] = { 0, };
		for (int j = 0; j < n_movies; j++)
		{
			if (movie[j].belong == i)
			{
				clusters[i].n_samples++;
				for (int k = 0; k < n_features; k++)
				{
					double value = (double)movie[j].vector[k];
					temp[k] += value;
				}
			}
		}
		for (int k = 0; k < n_features; k++)
		{
			clusters[i].centorid[k] = temp[k] / (double)clusters[i].n_samples;
		}
	}
}

void clustering()
{
	int k = 1;
	int n = 0;
	initialize_center();

	while (n!=30)
	{
		assign_label();
		compute_centroid();
		n++;
	}

}

void recommend_movie()
{
	int temp = 1, *save_arr = NULL;
	int num = 0, input_feature[n_features], temp_index = 0;
	
	printf("\n맘에드는 태그를 모두 입력하세요.\n1. 모험\n2. 애니메이션\n3. 유아용\n4. 코미디\n5. 판타지\n6. 멜로\n7. 드라마\n8. 액션\n9. 범죄\n10. 스릴러\n11. 공포\n12. 미스테리\n13. 공상과학소설\n14. I-MAX\n15. 다큐멘터리\n16. 전쟁\n17. 뮤지컬\n18. 모험\n19. 필름 느와르\n20. 서부개척\n21. 장르가 딱히 없는\n22. 고전영화\n23. 공상과학\n24. 오스카 상 수상작\n25. 활동적인\n26. 디즈니\n27. 다이어트 자극\n28. 개그\n29. 고화질 개봉\n30. 책이 원작\n31. 또 보고 싶은\n32. 별 감흥 없는\n33. 드라마 같은\n34. imdb top 250\n35. dvd로 보고싶은\n36. erlends dvds\n37. 간직하고 싶은\n38. 시간 여행\n39. 알몸 노출이 있는\n40. betamax\n41. 세계 2차 대전\n42. 원작 책이 있는\n43. 공상판타지\n44. 로맨스\n45. 미국 국립 영화 보존위원회 목록에 등재된 \n46. 반전있는 \n47. 분위기 있는 \n48. 책에서 찾을 수 있는\n49. 평점이 낮은\n50. 기억하고 싶은\n51. 소유하고 싶은\n52. 표준이 될 만한 \n53. 노출이 심한 \n54. 일본 애니메이션 \n55. 양식화된 \n56. 마약 \n57. 재밌는 \n58. 만화책\n59. 도서관에서 볼 법한\n60. 과대평가된\n61. 살인과 관련된\n62. dvd판으로 나온\n63. 범죄 \n64. 보고싶은 \n65. 영화관에서 봐야 할\n66. 긴장감 넘치는\n67. 조니뎁\n68. 충격적인\n69. 애니메이션\n70. 보고 싶은\n71. 리메이크작\n72. clearplay(Amazon 스트리밍)\n0.Exit");
	while (temp != 0)
	{
		scanf("%d", &temp);
		if (temp > 0 && temp < 73)
		{
			num++;
			save_arr = (int*)malloc(sizeof(int)*num);
			save_arr[num - 1] = temp - 1;
		}
		else if (temp != 0)
		{
			printf("잘못 입력하셨습니다.");
		}
		else
			break;
	}

	for (int i = 0; i < n_features; i++)
	{
		for (int j = 0; j < num; j++)
		{
			if (save_arr[j] == i)
				temp_index = 1;
		}
		if (temp_index == 1)
			input_feature[i] = 1;
		else
			input_feature[i] = 0;
	}
	double distance, result;
	int result_cluster = 0;
	for (int i = 0; i < n_clusters; i++)
	{
		
		distance = 0;
		for (int j = 0; j < n_features; j++)
		{
			distance += pow(clusters[i].centorid[j] - (double)input_feature[j], 2);
		}
		if (i==0)
			result = distance;
		if (distance < result)
		{
			result = distance;
			result_cluster = i;
		}
	}
	printf("\n추천 영화 목록");
	int j = 0;
	for (int i = 0; i < n_movies; i++)
	{
		if (movie[i].belong == result_cluster)
		{
			j++;
			printf("\n%d %s",j, movie[i].name);
		}
	}
}

void main()
{
	load_movie();
	load_tag();
	load_feature();
	clustering();
	recommend_movie();
}