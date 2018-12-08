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
	
	printf("\n������� �±׸� ��� �Է��ϼ���.\n1. ����\n2. �ִϸ��̼�\n3. ���ƿ�\n4. �ڹ̵�\n5. ��Ÿ��\n6. ���\n7. ���\n8. �׼�\n9. ����\n10. ������\n11. ����\n12. �̽��׸�\n13. ������мҼ�\n14. I-MAX\n15. ��ť���͸�\n16. ����\n17. ������\n18. ����\n19. �ʸ� ���͸�\n20. ���ΰ�ô\n21. �帣�� ���� ����\n22. ������ȭ\n23. �������\n24. ����ī �� ������\n25. Ȱ������\n26. �����\n27. ���̾�Ʈ �ڱ�\n28. ����\n29. ��ȭ�� ����\n30. å�� ����\n31. �� ���� ����\n32. �� ���� ����\n33. ��� ����\n34. imdb top 250\n35. dvd�� �������\n36. erlends dvds\n37. �����ϰ� ����\n38. �ð� ����\n39. �˸� ������ �ִ�\n40. betamax\n41. ���� 2�� ����\n42. ���� å�� �ִ�\n43. ������Ÿ��\n44. �θǽ�\n45. �̱� ���� ��ȭ ��������ȸ ��Ͽ� ����� \n46. �����ִ� \n47. ������ �ִ� \n48. å���� ã�� �� �ִ�\n49. ������ ����\n50. ����ϰ� ����\n51. �����ϰ� ����\n52. ǥ���� �� ���� \n53. ������ ���� \n54. �Ϻ� �ִϸ��̼� \n55. ���ȭ�� \n56. ���� \n57. ��մ� \n58. ��ȭå\n59. ���������� �� ����\n60. �����򰡵�\n61. ���ΰ� ���õ�\n62. dvd������ ����\n63. ���� \n64. ������� \n65. ��ȭ������ ���� ��\n66. ���尨 ��ġ��\n67. ���ϵ�\n68. �������\n69. �ִϸ��̼�\n70. ���� ����\n71. ������ũ��\n72. clearplay(Amazon ��Ʈ����)\n0.Exit");
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
			printf("�߸� �Է��ϼ̽��ϴ�.");
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
	printf("\n��õ ��ȭ ���");
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