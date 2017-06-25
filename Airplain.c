#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
//日期
typedef struct
{
	int year;//年
	int month;//月
	int day;//日
	int hour;//时
	int minute;//分
}DateData;
//子城市（主城市可直达的城市）
typedef struct CityData_son
{
	char name[10];//城市名称
	int length;//路程（单位：km）
	struct CityData_son *next;//此结构体指针
}CityData_son;
//主城市
typedef struct CityData
{
	char name[20];//城市名称
	char code[10];//城市代号 如：成都（CD）
	char abstract[200];//城市简介
	int ID;//城市编号
	struct CityData *next;//结构体指针
	CityData_son *airline;//可直达城市
}CityData;
//客户信息
typedef struct CusData
{
	char name[30];//姓名
	char id[20];//身份证
	char call[11];//电话
	char air_id[10];//航班编号
	int  seat;//座位号
	struct CusData *next;//此结构体指针
}CusData;
//航班信息
typedef struct AirplanData
{
	char name[10];//航班名称
	char id[10];//航班编号
	DateData startTime;//起飞时间
	DateData arriveTime;//到达时间
	CityData startCity;//起飞城市
	CityData arriveCity;//目的城市
	CusData *customer;//客户信息
	CusData *replace;//替补客户信息（最多6个替补客户）
	int seat;//已定座位数量
	int num;//机票数量
	int price;//机票价格
}AirplanData;
//航班信息链表（双向循环链表）
typedef struct AirplanNode
{
	AirplanData data;//航班基本信息
	struct AirplanNode *front;//前指针
	struct AirplanNode *next;//后指针
}LNode;
//定义链队
typedef struct Team
{
	LNode *data;//航班信息
	struct Team *next;//后指针
}Team;

//生成一个航班头结构体
LNode L;
//生成一个城市头结构体
CityData C;
//生成一个可直达城市头结构体
CityData_son Cson;
//生成一个乘客头结构体
CusData Cus;
//生成一个替补乘客头结构体
CusData Rep;

//图的操作
int visited[10];//表示（城市）顶点是否访问过
int d[10];//用于存放权值或储存路径顶点编号
#define Max   10//顶点最大数量
#define Infinity 5000 //两点为非邻结点时的权值

typedef struct
{
	CityData vexs[Max];//（城市）顶点信息
	int arcs[10][10];//邻接矩阵，用整数表示权值
	int arcNum;//边数
	int vexNum;//顶点数
}MGraph;
MGraph campus;//图变量
int r = 0;//判断是否成功导入航线图
MGraph InitGraph()//输入航线图
{
	int i = 0, j = 0;
	MGraph c;
	c.vexNum = Max;//顶点个数
	c.arcNum = 18;//边的个数
	for (i = 0; i<c.vexNum; i++)//依次设置编号
	{
		c.vexs[i].ID = i;
	}
	strcpy(c.vexs[0].name, "北京");
	strcpy(c.vexs[0].abstract, "北京，简称京，是中华人民共和国的首都、直辖市、国家中心城市、超大城市，全国政治中心、文化中心、国际交往中心、科技创新中心");
	strcpy(c.vexs[1].name, "上海");
	strcpy(c.vexs[1].abstract, "上海，简称沪或申，是中华人民共和国直辖市，国家中心城市,是中国第一大城市，中国的经济、交通、科技、工业、金融、贸易、会展和航运中心，首批沿海开放城市");
	strcpy(c.vexs[2].name, "香港");
	strcpy(c.vexs[2].abstract, " 香港，简称“港”，全称为中华人民共和国香港特别行政区（HKSAR）。香港地处中国华南，珠江口以东，南海沿岸，北接广东省深圳市、西接珠江，与澳门特别行政区隔着珠江口相望，其余两面与南海邻接。 ");
	strcpy(c.vexs[3].name, "海南");
	strcpy(c.vexs[3].abstract, "海南，简称琼，位于中国最南端。全省下辖4个地级市，5个县级市，4个县，6个自治县");
	strcpy(c.vexs[4].name, "成都");
	strcpy(c.vexs[4].abstract, "成都，简称蓉，四川省省会、副省级市，中国西南地区的科技、商贸、金融中心和交通枢纽");//一共五个城市 
	strcpy(c.vexs[5].name, "武汉");
	strcpy(c.vexs[5].abstract, "武汉，简称“汉”，中国湖北省省会 ，中国中部地区最大都市及唯一的副省级城市，中国内陆地区最繁华都市、中华人民共和国区域中心城市。");
	strcpy(c.vexs[6].name, "西安");
	strcpy(c.vexs[6].abstract, "西安，古称“长安”、“镐京”，陕西省省会，国家历史文化名城，2008年经国务院批复，被定位为国家重要的科研、教育和工业基地，我国西部地区重要的中心城市。 ");
	strcpy(c.vexs[7].name, "长沙");
	strcpy(c.vexs[7].abstract, "长沙市隶属湖南省，湖南省省会，简称长，地处湖南省东部偏北，湘江下游和湘浏盆地西缘，是全国两型社会建设综合配套改革试验区核心城市.");
	strcpy(c.vexs[8].name, "泸州");
	strcpy(c.vexs[8].abstract, "泸州市是中国历史文化名城，历史悠久。1983年3月3日国务院批复同意将地辖泸州市改为省辖市。");
	strcpy(c.vexs[9].name, "拉萨");
	strcpy(c.vexs[9].abstract, "拉萨是中国西藏自治区的首府，西藏的政治、经济、文化和宗教中心，也是藏传佛教圣地，拉萨位于西藏高原的中部、喜马拉雅山脉北侧，海拔3650米。");
	for (i = 0; i<c.vexNum; i++)
		for (j = 0; j<c.vexNum; j++)
			c.arcs[i][j] = Infinity;//初始化邻接矩阵
	c.arcs[0][3] = 2800; c.arcs[0][4] = 2000; c.arcs[0][5] = 1160;
	c.arcs[0][6] = 1000; c.arcs[1][3] = 2100; c.arcs[1][5] = 1000;
	c.arcs[1][2] = 1300; c.arcs[2][5] = 920;  c.arcs[2][7] = 850;
	c.arcs[3][5] = 1600; c.arcs[4][6] = 720;  c.arcs[4][7] = 1200;
	c.arcs[4][9] = 2200; c.arcs[5][7] = 400;  c.arcs[5][8] = 1015;
	c.arcs[6][7] = 1000; c.arcs[6][9] = 2800; c.arcs[7][8] = 1010;//初始化各个城市之间的距离
	for (i = 0; i<c.vexNum; i++)
		for (j = 0; j<c.vexNum; j++)//邻接矩阵对称赋值
			c.arcs[j][i] = c.arcs[i][j];
	printf("\t输入航线图成功\n"); r = 1;
	return c;
}//InitGraph
void IntroduceCompus(MGraph c)//查看航线图
{
	int i, j;
	if (r == 0)
		printf("请先输入航线图\n");
	else
	{
		printf("\n\n编号		城市名称				简介\n");
		printf("________________________________________\n");
		for (i = 0; i<c.vexNum; i++)
		{
			printf("%-10d%13s%\n%50s\n", c.vexs[i].ID, c.vexs[i].name, c.vexs[i].abstract);
			printf("________________________________________\n\n");
		}

		printf("从左至右编号0~N，从上至下编号0~N\n");
		printf("图的邻接矩阵为下图，其中数字为站与站之间的距离 ，5000表示无法直接到达或者为此站本身\n");
		for (i = 0; i<Max; i++)
		{
			for (j = 0; j<Max; j++)
			{
				printf("%7d", c.arcs[i][j]);
			}
			printf("\n");
		}
	}//end else
}
void Search(int i);
//用floyd算法，求各个城市v,w之间的最短路径p[][]及其带权长度d[v][w]
//若p[v][w][u]==1,则u是v到w当前的最短路径上的顶点
void ShortestPath(MGraph c)
{
	int i, j, k, v, u, w, d[10][10], p[10][10][10];//i,j，k为普通变量，v,u,w为遍历变量
	int e;//判断是否查询当前航班
	if (r == 0)
		printf("请先输入航线图\n");
	else
	{
		for (v = 0; v<c.vexNum; v++)
		{
			for (w = 0; w<c.vexNum; w++)
			{
				d[v][w] = c.arcs[v][w];//d[v][w]中存放v至w的权值（路径长度）
				//初始化最短路径p[v][w][]数组，第三个分量全部清0
				for (u = 0; u<c.vexNum; u++)
					p[v][w][u] = 0;
				if (d[v][w]<Infinity)//如果v至w有边相连
				{
					p[v][w][v] = 1;   //v是v至w上最短路径上的顶点
					p[w][v][w] = 1;	//w是v至w上最短路径上的顶点
				}//if
			}//for
		}//end for
		for (u = 0; u<c.vexNum; u++)//求v至w的最短路径及其距离
		{						//对任意顶点u，试探其是否为v至w最短路径上的顶点
			for (v = 0; v<c.vexNum; v++)
			for (w = 0; w<c.vexNum; w++)//判断从v经过u到w的一条路更短
			if (d[v][u] + d[u][w]<d[v][w])
			{
				d[v][w] = d[v][u] + d[u][w];//修改v到u的最短路径
				for (i = 0; i<c.vexNum; i++)//修改v至w的最短路径数组
					//若i是v至的最短路径上的顶点，或i是u至w的最短路径上的顶点
					//则i是v至w上最短路径的顶点
					p[v][w][i] = p[v][u][i] || p[u][w][i];
			}
		}//end for
		printf("\n\n编号		城市名称	\n");
		printf("_____________________________\n");
		for (i = 0; i<c.vexNum; i++)
		{
			printf("%-10d%13s%\n", c.vexs[i].ID, c.vexs[i].name);
		}
		printf("\n请分别输入出发城市和目的城市的编号：");
		scanf("%d%d", &k, &j);
		printf("\n\n");
		while (k<0 || k>c.vexNum || j<0 || j>c.vexNum)
		{
			printf("\n您所输入的城市编号不存在!");
			printf("\n请重新输入出发城市和目的城市的编号：\n\n");
			scanf("%d%d", &k, &j);
			printf("\n\n");
		}
		printf("最短路径为:\n");
		printf("%s", c.vexs[k].name);//输出出发城市的名字
		for (u = 0; u<c.vexNum; u++)
		{
			if (p[k][j][u] && k != u&&j != u)//输出最短路径上中间城市名称
					printf("--->%s", c.vexs[u].name);
		}
		printf("--->%s", c.vexs[j].name);//输出目的城市的名称
		printf("\n\n\n总长为%d公里\n", d[k][j]);
		printf("请问是否查询已有航班信息(输入'1'查询，输入'0'返回上一级)：\n");
		scanf("%d", &e);
		while (e == 1)
		{
			Search(5);
			printf("请问是否继续查询已有航班信息(输入'1'查询，输入'0'返回主菜单)：\n");
			scanf("%d", &e);
		}
	}//end else
}

//加载已有数据
void LodeData()
{
	FILE *fp_air, *fp_city, *fp_cityson, *fp_customer, *fp_replace;//fp_air 航班信息文件地址 fp_city 主城市文件地址 fp_cityson 子城市文件地址
	CityData *Header_city = &C, *q_city;//q_city 用于空间分配  Header_city 指向头并用于插入操作
	CityData_son *q_cityson, *Header_cityson, *temp = &Cson, *free;//q_cityson 用于空间分配  Header_cityson 指向头并用于插入操作 temp 临时结构参与子城市与主城市的连接 free 控制每一段子城市结束后将连边断开  
	CusData *q_customer, *q_replace, *Header_customer = &Cus, *Header_replace = &Rep, *temp_c = &Cus, *temp_r = &Rep, *free_c, *free_r;//q_customer q_replace用于空间分配													  
	LNode *q_air, *Footer_air = L.front, *Header_air = &L;//临时指针*q  尾指针*Footer_air 头指针*Header_air
	Header_cityson = &Cson;//初始化
	/*-------初始化航班信息-------*/
	if (0 == access("data.txt", 0))//判断文件是否存在
	{
		if ((fp_air = fopen("data.txt", "rb")) == NULL) //打开航班信息文件并检查是否成功
		{
			printf("1");
			system("pause");
			return;//打开文件并检查是否成功
		}
		do
		{
			//分配空间
			q_air = (LNode *)malloc(sizeof(LNode));
			if (q_air != NULL)
			{
				//从文件一条一条的读取数据
				if (1 == fread(&(q_air->data), sizeof(AirplanData), 1, fp_air))
				{
					//将数据依次插入链表最后
					Header_air->front = q_air;
					Footer_air->next = q_air;
					q_air->front = Footer_air;
					q_air->next = Header_air;
					Footer_air = Footer_air->next;
				}
				else q_air = NULL;
			}
		} while (q_air != NULL);
		fclose(fp_air);//关闭文件
	}
	/*-------初始化乘客信息-------*/
	if ((fp_customer = fopen("customer_data.txt", "rb")) != NULL)
	{
		do
		{
			//分配空间
			q_customer = (CusData *)malloc(sizeof(CusData));
			if (q_customer != NULL)
			{
				//获取数据
				if (1 == fread(q_customer, sizeof(CusData), 1, fp_customer))
				{
					//数据连接
					q_customer->next = NULL;
					Header_customer->next = q_customer;
					Header_customer = Header_customer->next;
				}
				else q_customer = NULL;
			}
		} while (q_customer != NULL);
	}
	/*-------将乘客与航班相连-------*/
	q_air = &L;//航班
	q_air = q_air->next;
	q_customer = &Cus;//乘客
    q_customer = q_customer->next;
	if (q_air != &L)
	{
		do
		{
			if (strcmp(temp_c->next->name, "#") == 0)
			{
				//乘客与航班相连
				q_air->data.customer = q_customer;
				free_c = temp_c;
				temp_c = temp_c->next;
				free_c->next = NULL;
				q_customer = temp_c->next;
				q_air = q_air->next;//下一个航班
			}
			else temp_c = temp_c->next;//下一个乘客
		} while (q_air != &L);//结束条件
	}
	/*-------初始化替补客户信息-------*/
	if ((fp_replace = fopen("replace_data.txt", "rb")) != NULL)
	{
		do
		{
			//分配空间
			q_replace = (CusData *)malloc(sizeof(CusData));
			if (q_replace != NULL)
			{
				//获取数据
				if (1 == fread(q_replace, sizeof(CusData), 1, fp_replace))
				{
					//数据连接
					q_replace->next = NULL;
					Header_replace->next = q_replace;
					Header_replace = Header_replace->next;
				}
				else q_replace = NULL;
			}
		} while (q_replace != NULL);
	}
	/*-------将替补客户与航班相连-------*/
	q_air = &L;//航班
	q_air = q_air->next;
	q_replace = &Rep;//乘客
	q_replace = q_replace->next;
	if (q_air != &L)
	{
		do
		{
			if (strcmp(temp_r->next->name, "#") == 0)
			{
				//替补客户与航班相连
				q_air->data.replace = q_replace;
				free_r = temp_r;
				temp_r = temp_r->next;
				free_r->next = NULL;
				q_replace = temp_r->next;
				q_air = q_air->next;//下一个航班
			}
			else temp_r = temp_r->next;//下一个乘客
		} while (q_air != &L);//结束条件
	}
	/*-------初始化主城市信息-------*/
	if (0 == access("citydata.txt", 0))//判断文件是否存在
	{
		if ((fp_city = fopen("citydata.txt", "rb")) == NULL) return;//打开主城市文件并检查是否成功		
		do
		{
			//分配空间
			q_city = (CityData *)malloc(sizeof(CityData));
			if (q_city != NULL)
			{
				if (1 == fread(q_city, sizeof(CityData), 1, fp_city))
				{	//将数据依次插入链表最后
					q_city->next = NULL;
					q_city->airline = NULL;
					Header_city->next = q_city;
					Header_city = Header_city->next;
				}
				else q_city = NULL;
			}
		} while (q_city != NULL);
		fclose(fp_city);//关闭文件
		/*-------初始化子城市信息-------*/
		if ((fp_cityson = fopen("citydata_son.txt", "rb")) == NULL) return;//打开子城市文件并检查是否成功
		do
		{
			//分配空间
			q_cityson = (CityData_son *)malloc(sizeof(CityData_son));
			if (q_cityson != NULL)
			{
				//从文件一条一条的读取数据
				if (1 == fread(q_cityson, sizeof(CityData_son), 1, fp_cityson))
				{
					q_cityson->next = NULL;
					Header_cityson->next = q_cityson;
					Header_cityson = Header_cityson->next;
				}
				else q_cityson = NULL;
			}
		} while (q_cityson != NULL);
		fclose(fp_cityson);
	}
	else
	{
		printf("\t文件不存在，请联系维护人员\n");
		system("pause");
	}
	/*-------将子城市与主城市相连-------*/
	q_city = &C;//指向主城市
	q_city = q_city->next;//指向第一个数据
	q_cityson = temp;//q_cityson 指向首元素并参与下面连接过程
	q_cityson = q_cityson->next;//指向第一个元素
	if (q_city != NULL)
	{
		do
		{
			if (strcmp(temp->next->name, "#") == 0)//遇到# 则拿到一个主城市的所有直达城市
			{
				//将子城市数据连在主城市后面
				q_city->airline = q_cityson;
				free = temp;
				temp = temp->next;
				free->next = NULL;
				q_cityson = temp->next;
				q_city = q_city->next;//下一个城市
			}
			else temp = temp->next;
		} while (temp != NULL && q_city != NULL); //结束条件
	}
}
void save(int choice)
{
	int i;
	FILE *fp_air, *fp_city, *fp_cityson, *fp_customer, *fp_replace;//fp_air 航班信息文件地址 fp_city 主城市文件地址 fp_cityson 子城市文件地址
	LNode *p_air = &L;//p_air 指向航班信息
	CityData *p_city = &C;//p_city 指向主城市信息
	CityData_son temp, *p_cityson;//p_cityson 指向子城市信息 temp 储存字符"#"
	CusData *p_customer = &Cus, *p_replace = &Rep, Cus_temp;//p_customer 指向乘客信息 p_replace 指向替补客户信息 Cus_temp 储存字符“#”
	strcpy(temp.name, "#");
	strcpy(Cus_temp.name, "#");
	/*-------保存航班信息-------*/
	if (choice == 1)
	{
		if (0 == access("data.txt", 0))//判断文件是否存在
		{
			if ((fp_air = fopen("data.txt", "wb")) == NULL) //打开文件并检查是否成功
			{
				printf("\t打开文件失败，请重试或者联系维护人员\n");
				printf("\t按‘0’退出程序,按‘1’继续操作");
				scanf("%d", &choice);
				if (choice == 0)
				{
					fclose(fp_air);//关闭文件
					return;//‘0’结束程序
				}
				printf("\t请稍后，正在重试中......\n");
				fclose(fp_air);//关闭文件
				return save(1);//继续尝试打开文件
			}
			//保存数据
			else
			{
				//删除原有文件内容
				fp_customer = fopen("customer_data.txt", "wb");
				fclose(fp_customer);
				fp_replace = fopen("replace_data.txt", "wb");
				fclose(fp_replace);
				p_air = p_air->next;//指向第一个航班
				while (p_air != &L)
				{
					//指向乘客信息
					p_customer = &Cus;
					p_customer->next = p_air->data.customer;
					p_customer = p_customer->next;
					//指向替补乘客信息
					p_replace = &Rep;
					p_replace->next = p_air->data.replace;
					p_replace = p_replace->next;
					if (fwrite(&(p_air->data), sizeof(AirplanData), 1, fp_air) != 1)//一条一条写入航班数据
					{
						printf("\t文件写入错误！请重试或者联系维护人员\n");
						printf("\t按‘0’退出程序,按‘1’继续操作");
						scanf("%d", &choice);
						if (choice == 0)return;//‘0’结束程序
						printf("\t请稍后，正在重试中......\n");
						return save(choice);//继续尝试打开文件
					}
					else
					{
						//打开乘客信息文件
						if ((fp_customer = fopen("customer_data.txt", "ab+")) != NULL)
						{
							while (p_customer != NULL)
							{
								//写入乘客信息
								if (fwrite(p_customer, sizeof(CusData), 1, fp_customer) != 1)
								{
									printf("\t乘客文件写入错误！请重试或者联系维护人员\n");
									printf("\t按‘0’退出程序,按‘1’继续操作");
									scanf("%d", &choice);
									if (choice == 0)return;//‘0’结束程序
									printf("\t请稍后，正在重试中......\n");
									return save(choice);//继续尝试打开文件
								}
								p_customer = p_customer->next;//指向下一个
							}
						}
						fwrite(&Cus_temp, sizeof(CusData), 1, fp_customer);
						//关闭文件
						fclose(fp_customer);
						//打开替补乘客文件
						if ((fp_replace = fopen("replace_data.txt", "ab+")) != NULL)
						{
							while (p_replace != NULL)
							{
								//写入替补乘客信息
								if (fwrite(p_replace, sizeof(CusData), 1, fp_replace) != 1)
								{
									printf("\t乘客文件写入错误！请重试或者联系维护人员\n");
									printf("\t按‘0’退出程序,按‘1’继续操作");
									scanf("%d", &choice);
									if (choice == 0)return;//‘0’结束程序
									printf("\t请稍后，正在重试中......\n");
									return save(choice);//继续尝试打开文件
								}
								p_replace = p_replace->next;//指向下一个
							}
						}
						fwrite(&Cus_temp, sizeof(CusData), 1, fp_replace);
						//关闭文件
						fclose(fp_replace);
						p_air = p_air->next;//指向下一个航班
						//system("pause");
					}
				}
				printf("\t信息储存成功\n");
				system("pause");
				fclose(fp_air);//关闭文件
			}
		}
		else
		{
			printf("\t文件不存在，请联系维护人员\n");
			system("pause");
		}
	}
	/*-------保存城市信息-------*/
	else if (choice == 2)
	{
		if (0 == access("citydata.txt", 0))//判断文件是否存在
		{
			if ((fp_city = fopen("citydata.txt", "wb")) == NULL) //打开主城市信息文件并检查是否成功
			{
				printf("\t打开文件失败，请重试或者联系维护人员\n");
				printf("\t按‘0’退出程序,按‘1’继续操作");
				scanf("%d", &choice);
				if (choice == 0)
				{
					fclose(fp_city);//关闭文件
					return;//‘0’结束程序
				}
				printf("\t请稍后，正在重试中......\n");
				fclose(fp_city);//关闭文件
				return save(2);//继续尝试打开文件
			}
			//保存数据
			else
			{
				remove("citydata_son.txt");//删除子城市的原有文件
				p_city = p_city->next;//指向第一个城市
				while (p_city != NULL)
				{
					p_cityson = p_city->airline;
					if (fwrite(p_city, sizeof(CityData), 1, fp_city) == 1)//一条一条写入主城市数据
					{
						if ((fp_cityson = fopen("citydata_son.txt", "ab")) == NULL) //打开子城市文件并检查是否成功(追加模式)
						{
							printf("\t打开文件失败，请重试或者联系维护人员\n");
							printf("\t按‘0’退出程序,按‘1’继续操作");
							scanf("%d", &choice);
							if (choice == 0)
							{
								fclose(fp_cityson);//关闭文件
								return;//‘0’结束程序
							}
							printf("\t请稍后，正在重试中......\n");
							fclose(fp_cityson);//关闭文件
							return save(2);//继续尝试打开文件
						}
						else
						{
							while (p_cityson != NULL)
							{
								if (fwrite(p_cityson, sizeof(CityData_son), 1, fp_cityson) != 1)//一条一条写入子城市数据
								{
									printf("\t文件写入错误！请重试或者联系维护人员\n");
									printf("\t按‘0’退出程序,按‘1’继续操作");
									scanf("%d", &choice);
									if (choice == 0)return;//‘0’结束程序
									printf("\t请稍后，正在重试中......\n");
									return save(choice);//继续尝试打开文件
								}
								p_cityson = p_cityson->next;//指向下一个子城市
							}
							fwrite(&temp, sizeof(CityData_son), 1, fp_cityson);//temp = "#" 结束符
							fclose(fp_cityson);
						}
					}
					else
					{
						printf("\t文件写入错误！请重试或者联系维护人员\n");
						printf("\t按‘0’退出程序,按‘1’继续操作");
						scanf("%d", &choice);
						if (choice == 0)return;//‘0’结束程序
						printf("\t请稍后，正在重试中......\n");
						return save(2);//继续尝试打开文件
					}
					p_city = p_city->next;//指向下一个城市
				}
				printf("\t信息储存成功\n");
				system("pause");
				fclose(fp_city);//关闭文件
			}
		}
		else
		{
			printf("\t文件不存在，请联系维护人员\n");
			system("pause");
		}
	}
}
//航班输出的格式
void input(LNode *p, int choice)//choice 序号
{
	int temp = -1;//储存剩余票数
	temp = p->data.num - p->data.seat;//计算剩余票数
	printf("\t%d\t%s\t%-8s\t%-4s\t%-4s\t%d-%d-%d %d:%d\t\t%d-%d-%d %d:%d\t\t%d\t  %d\t\t%d\n", choice, p->data.name, p->data.id, p->data.startCity.name, p->data.arriveCity.name,
		p->data.startTime.year, p->data.startTime.month, p->data.startTime.day, p->data.startTime.hour, p->data.startTime.minute,
		p->data.arriveTime.year, p->data.arriveTime.month, p->data.arriveTime.day, p->data.arriveTime.hour, p->data.arriveTime.minute, p->data.num, temp, p->data.price);
}
//增加航班
void Insert()
{
	LNode *p, *Footer = L.front, *Header = &L;//临时指针*p  尾指针*Footer 头指针*Header
	char startCity[10], arriveCity[10], airId[10];//startCity:出发地 arriveCity:目的地 airId:航班编号
	printf("\t请输入新增航线的航班编号：");
	scanf("%s", airId);
	printf("\t请输入出发地：");
	scanf("%s", startCity);
	printf("\t请输入目的地：");
	scanf("%s", arriveCity);
	Header = Header->next;//指向第一个数据
	//检查该线路是否已经存在准备新增的航班
	while (Header != &L)
	{
		if (strcmp(Header->data.id, airId) == 0)
		{
			if (strcmp(Header->data.startCity.name, startCity) == 0)
			{
				if (strcmp(Header->data.arriveCity.name, arriveCity) == 0)
				{
					printf("\t该航班存在该线路,请重新输入航线\n");
					system("pause");
					return Insert();
				}
				else Header = Header->next;//向后移一位
			}
			else Header = Header->next;//向后移一位
		}
		else Header = Header->next;//向后移一位
	}
	//添加航班
	p = (LNode *)malloc(sizeof(LNode));
	//验证空间是否生成
	if (p == NULL)
	{
		printf("\t空间生成失败，请重试或者联系维护人员\n");
		system("pause");
		return;
	}
	//添加数据
	else
	{
		printf("\t请输入航班名称：");
		scanf("%s", p->data.name);
		printf("\t请输入起飞时间（如：2017 06 17 16 30）：");
		scanf("%d%d%d%d%d", &p->data.startTime.year, &p->data.startTime.month, &p->data.startTime.day, &p->data.startTime.hour, &p->data.startTime.minute);
		printf("\t请输入到达时间（如：2017 06 17 16 30）：");
		scanf("%d%d%d%d%d", &p->data.arriveTime.year, &p->data.arriveTime.month, &p->data.arriveTime.day, &p->data.arriveTime.hour, &p->data.arriveTime.minute);
		printf("\t请输入预出售机票数量：");
		scanf("%d", &p->data.num);
		printf("\t请输入预售机票价格：");
		scanf("%d", &p->data.price);
		p->data.seat = 0;
		strcpy(p->data.id, airId);
		strcpy(p->data.startCity.name, startCity);
		strcpy(p->data.arriveCity.name, arriveCity);
		p->data.customer = NULL;
		p->data.replace = NULL;
	}
	Header = &L;
	//添加到链表最后面并且储存
	Header->front = p;
	Footer->next = p;
	p->front = Footer;
	p->next = Header;
	save(1);//将航班信息储存在本地
}
//修改航班
void Update(Team *T, int len)
{
	void Edit();//引用子菜单函数
	int choice, temp = 1, data = -1;//choice 序号 temp 循环控制 data 暂存数据
	char string[10] = { '0' };//暂存数据
	Team *p, *q;//临时指针
	p = T; p = p->next;//初始化指针并指向队的第一个元素
	printf("\t请输入需要修改的航班的序号：");
	scanf("%d", &choice);
	if (choice <= len&&choice>0)
	{
		while (temp<choice)
		{
			p = p->next; temp++;
		}
		if(p->data->data.seat!=0)
		{
			printf("\t该航班有乘客，无法进行删除操作!!!\n\t");
			system("pause");
			return;
		}
		do
		{
			printf("\t编号\t  1 \t\t  2 \t\t  3 \t  4 \t   5    \t\t   6    \t\t   7  \t    8   \t  9 \t 10 \t 0  \n");
			printf("\t序号\t名称\t\t编号\t\t起点\t终点\t起飞时间\t\t到达时间\t\t总票数\t剩余票数\t价格\t删除\t取消\n\n");
			input(p->data, choice);
			printf("\n\t请输入对应编号进行操作：");
			scanf("%d", &data);
			switch (data)
			{
				case 1:printf("\t请输入新的航班名称：");
					strcpy(string, p->data->data.name);
					scanf("%s", p->data->data.name); break;
				case 2:printf("\t请输入新的航班编号：");
					strcpy(string, p->data->data.id);
					scanf("%s", p->data->data.id); break;
				case 3:printf("\t请输入新的起飞地：");
					strcpy(string, p->data->data.startCity.name);
					scanf("%s", p->data->data.startCity.name); break;
				case 4:printf("\t请输入新的航班目的地：");
					strcpy(string, p->data->data.arriveCity.name);
					scanf("%s", p->data->data.arriveCity.name); break;
				case 5:printf("\t请输入新的时间（如：2017 06 16 16 30）：");
					scanf("%d%d%d%d%d", &p->data->data.arriveTime.year, &p->data->data.arriveTime.month, &p->data->data.arriveTime.day, &p->data->data.arriveTime.hour, &p->data->data.arriveTime.minute); break;
				case 6:printf("\t请输入新的时间（如：2017 06 16 16 30）：");
					scanf("%d%d%d%d%d", &p->data->data.startTime.year, &p->data->data.startTime.month, &p->data->data.startTime.day, &p->data->data.startTime.hour, &p->data->data.startTime.minute); break;
				case 7:printf("\t请输入新的总数：");
					temp = p->data->data.num;
					scanf("%d", &p->data->data.num);
				case 8:break;
				case 9:printf("\t请输入新的价格：");
					temp = p->data->data.price;
					scanf("%d", &p->data->data.price); break;
				case 10:printf("\t确认删除请按‘1’，取消请按‘0’或其他");
					scanf("%d", &temp);
					if (temp == 1)
					{
						q = p;
						p->data->front->next = p->data->next;
						p->data->next->front = p->data->front;
						p = p->next;
						free(q);
						printf("\t删除成功\n\t");
						system("pause");
					}
					else
					{
						printf("\t取消操作成功\n\t");
						system("pause");
					}break;
				case 0:Edit(); break;//取消操作之后返回子菜单
				default:printf("\t请输入正确编号：");
			}
			if (data != 10)
			{
				system("cls");
				printf("\n\t修改后数据\n");
				printf("\t序号\t名称\t\t编号\t\t起点\t终点\t起飞时间\t\t到达时间\t\t总票数\t剩余票数\t价格n\n");
				input(p->data, choice);
			}
			do
			{
				printf("\n\t继续修改请按‘1’，保存并退出请按‘2’，放弃并退出请按‘0’：");
				scanf("%d", &choice);
				if (choice>2)
				{
					printf("\t请按正确编号操作！\n\t");
					system("pause");
				}
			} while (choice>2);
		} while (choice == 1);
		if (choice == 2)
		{
			save(1);
			printf("\t保存成功\n\t");
			system("pause");
			Edit();//保存之后返回子菜单
		}
		else
		{
			if (data == 1)strcpy(p->data->data.name, string);
			if (data == 2)strcpy(p->data->data.id, string);
			if (data == 3)strcpy(p->data->data.startCity.name, string);
			if (data == 4)strcpy(p->data->data.arriveCity.name, string);
			if (data == 7)p->data->data.num = temp;
			if (data == 9)p->data->data.price = temp;
			if (data == 10)
				printf("\t已取消修改\n\t");
			system("pause");
			Edit();//放弃修改返回子菜单
		}
	}
	else
	{
		printf("\t请重新输入正确序号\n\t");
		system("pause");
		Update(T, len);
	}
}
//删除航班
void Delete(Team *T, int len)
{
	void Edit();//引用子菜单函数
	int choice, temp = 1;//choice 序号 temp 循环控制
	Team *p, *q;//临时指针
	p = T; p = p->next;//初始化指针并指向队的第一个元素
	printf("\t请输入需要删除的航班的序号：");
	scanf("%d", &choice);
	if (choice <= len&&choice>0)
	{
		while (temp<choice)
		{
			p = p->next; temp++;
		}
		//显示所找航班信息
		printf("\t序号\t名称\t\t编号\t\t起点\t终点\t起飞时间\t\t到达时间\t\t总票数\t剩余票数\t价格\n\n");
		input(p->data, choice);
		printf("\t确认删除请按‘1’，放弃请按‘0’");
		scanf("%d", &choice);
		if (choice == 1)
		{
			if(p->data->data.seat!=0)
			{
				printf("\t该航班有乘客，无法进行删除操作!!!\n\t");
				system("pause");
				return;
			}
			//删除航班
			q = p;
			p->data->front->next = q->data->next;
			p->data->next->front = q->data->front;
			printf("\t已经成功删除 %s 航班\n", p->data->data.id);
			save(1);
			free(q);//释放内存
		}
		else
		{
			printf("\t取消操作成功\n\t");
			Edit();//调用子菜单，实现循环
		}
	}
	else
	{
		printf("\t请重新输入正确序号\n\t");
		system("pause");
		Delete(T, len);//调用自己，实现循环
	}
}
//查询单个航班信息
void Search(int i)//待测试
{
	void Edit();
	void Menu();
	void Reserve(Team *T, int len);
	int choice, len = 0;//choice编号 len 队的长度
	LNode *Header = &L;// 头指针*Header
	Team T, *p, *q;//队T  临时队*p *q
	char startCity[10], arriveCity[10], airId[10];//startCity:出发地 arriveCity:目的地 airId:航班编号
	Header = Header->next;//指向第一个数据
	T.next = NULL; q = &T;//初始化数据
	printf("\t输入航班编号查询请按‘0’，输入出发地和目的地查询请按‘1’，取消请按‘2’：");
	scanf("%d", &choice);
	if (choice == 0)
	{
		printf("\t请输入航班编号：");
		scanf("%s", airId);
		printf("\t序号\t名称\t\t编号\t\t起点\t终点\t起飞时间\t\t到达时间\t\t总票数\t剩余票数\t价格\n\n");
		while (Header != &L)
		{
			if (strcmp(Header->data.id, airId) == 0)
			{
				p = (Team *)malloc(sizeof(Team));//生成队的一个数据
				p->data = Header;
				q->next = p; q = q->next; q->next = NULL; len++;//添加数据在队尾
				input(Header, ++choice);//输出当前信息
				Header = Header->next;//指向下一个数据
			}
			else	Header = Header->next;//指向下一个数据
		}
		if (T.next == NULL)
		{
			system("cls");
			printf("\n\n");
			printf("\t没有找到任何相关信息，请核对输入的信息！\n\t");
			system("pause");
			Search(i);//调用自己，实现循环
		}
		else
		{
			if (i == 3) Delete(&T, len);//删除
			else if (i == 2) Update(&T, len);//修改
			else if (i == 4) Reserve(&T, len);//订票
		}
	}
	else if (choice == 1)
	{
		printf("\t请输入出发地：");
		scanf("%s", startCity);
		printf("\t请输入目的地：");
		scanf("%s", arriveCity);
		printf("\t序号\t名称\t\t编号\t\t起点\t终点\t起飞时间\t\t到达时间\t\t总票数\t剩余票数\t价格\n\n");
		while (Header != &L)
		{
			if (strcmp(Header->data.startCity.name, startCity) == 0)//查找出发地
			{
				if (strcmp(Header->data.arriveCity.name, arriveCity) == 0)//查找目的地
				{
					p = (Team *)malloc(sizeof(Team));//生成队的一个数据
					p->data = Header;
					q->next = p; q = q->next; q->next = NULL; len++;//添加数据在队尾
					input(Header, choice++);//输出当前信息
					Header = Header->next;//指向下一个数据
				}
				else Header = Header->next;//指向下一个数据
			}
			else Header = Header->next;//指向下一个数据
		}
		if (T.next == NULL)
		{
			system("cls");
			printf("\n\n");
			printf("\t没有找到任何相关信息，请核对输入的信息！\n\t");
			system("pause");
			Search(i);//调用自己，实现循环
		}
		else
		{
			if (i == 3) Delete(&T, len);//删除
			else if (i == 2) Update(&T, len);//修改
			else if (i == 4) Reserve(&T, len);//订票
			else system("pause");
		}
	}
	else if (choice == 2)
	{
		if (i == 4) return Menu();
		else Edit();
	}
	else
	{
		printf("\t请输入正确编码\n\t");
		system("pause");
		Search(i);//调用自己，实现循环
	}
}
//编辑航班信息
void Edit()//子菜单
{
	int choice;//存储编号
	system("cls");
	printf("\t----------------------------\n");
	printf("\t|     航班信息管理         |\n");
	printf("\t|--------------------------|\n");
	printf("\t|功能：1、增加航线         |\n");
	printf("\t|功能：2、修改航线         |\n");
	printf("\t|功能：3、删除航线         |\n");
	printf("\t|功能：9、返回上一层       |\n");
	printf("\t|功能：0、退出             |\n");
	printf("\t----------------------------\n");
	printf("\t请输入编号：");
	scanf("%d", &choice);
	switch (choice)
	{
		case 1:Insert(); break;//Insert 增加
		case 2:Search(2); break;//Update 修改
		case 3:Search(3); break;//Delete 删除
		case 9:break;//返回上一层
		case 0:exit(1);//退出
		default:printf("\t输入编号错误，请按给定编号输入\n\t"); system("pause"); return Edit();//提示错误信息并返回Edit（）函数
	}
}
//显示所有航班信息
void Display()
{
	int choice = 0;
	LNode *p = &L;
	p = p->next;//指向第一个数据
	printf("\t序号\t名称\t\t编号\t\t起点\t终点\t起飞时间\t\t到达时间\t\t总票数\t剩余票数\t价格\n\n");
	while (p != &L)
	{
		input(p, ++choice);
		p = p->next;
	}
	if (choice == 0)
	{
		system("cls");
		printf("\t没有任何数据\n\t");
	}
	printf("\t"); system("pause");
}
//城市信息输出格式
void input_city(CityData *C, int choice)
{
	CityData_son *p;//临时结构体
	p = &Cson;
	p->next = C->airline;
	p = p->next;
	printf("\t序号\t名称\t\t代号\t介绍\n\n");
	printf("\t%d\t%s\t\t%s\t%s\n", choice, C->name, C->code, C->abstract);
	printf("\t可直达城市:\n");
	if (p == NULL)choice = -1;
	printf("\t\t名称\t距离\n\n");
	while (p != NULL)//结束条件
	{
		printf("\t\t%s\t%d\n", p->name, p->length);
		p = p->next;

	}
	printf("\n");
	if (choice == -1)printf("\t\t无\n\n");
}
//显示已有城市信息
void Display_city()
{
	void Edit_city();
	int choice = 0;
	CityData *p = &C;
	p = p->next;//指向第一个数据;
	while (p != NULL)
	{
		input_city(p, ++choice);
		p = p->next;
	}
	if (choice == 0)
	{
		system("cls");
		printf("\t没有任何数据\n\t");
	}
	printf("\t"); system("pause");
	return Edit_city();
}
//添加城市信息
void Insert_city()
{
	void Edit_city();//加载子菜单
	CityData *p, *q;//临时指针
	CityData_son *Header, *s;//临时指针
	char temp[10];//临时存储数据
	p = &C; Header = &Cson;//初始化
	printf("\t请输入需要填加的城市名：");
	scanf("%s", temp);
	while (p->next != NULL)
	{
		if (strcmp(p->name, temp) == 0)
		{
			printf("\t已经存在该城市\n\t");
			input_city(p, 1);
			printf("\t请重新输入！\n\t");
			system("pause");
			Insert_city();
		}
		else p = p->next;//指针下移
	}
	q = (CityData *)malloc(sizeof(CityData));
	if (q == NULL)return;
	strcpy(q->name, temp);
	printf("\t请输入城市代号：");
	scanf("%s", q->code);
	printf("\t请输入城市简介：");
	scanf("%s", q->abstract);
	//输入可直达城市
	do
	{
		printf("\t请输入可直达城市：");
		s = (CityData_son *)malloc(sizeof(CityData_son));
		if (s == NULL)return;
		scanf("%s", s->name);
		printf("\t实际距离（单位：km）：");
		scanf("%d", &s->length);
		s->next = NULL;
		Header->next = s; Header = Header->next;//将可直达城市数据链接起来
		printf("\t继续添加请按‘1’，停止请按‘0’：");
		scanf("%s", temp);
	} while (strcmp(temp, "0") != 0);//结束条件
	q->airline = Cson.next;//与城市信息连接起来
	q->next = NULL;
	p->next = q;//将城市信息连接在链表后面
	printf("\t添加成功\n");
	printf("\t继续添加请按‘1’，返回上一层请按‘0’：");
	scanf("%s", temp);
	if (strcmp(temp, "1") == 0) Insert_city();
	else
	{
		save(2);//保存城市信息
		Edit_city();
	}
}
//删除城市
void Delete_city(CityData *q, CityData *p)
{
	void Edit_city();
	q->next = p->next;
	printf("\t删除“%s”成功\n\t", p->name);
	free(p);
	save(2);
	return Edit_city();
}
//修改城市信息
void Update_city(CityData *p, int choice)
{
	void Edit_city();
	int num;//接收编号
	char temp[100], temp_1[100];//修改信息是暂存数据	
	CityData_son *q = &Cson;
	q = q->next;
	system("cls");
	printf("\t编号\t1 号\t\t2 号\t3 号\n");
	input_city(p, choice);//输出结果	
	printf("\t请输入对应编号修改信息（返回上一层请按‘0’）：");
	scanf("%d", &num);
	switch (num)
	{
		case 1:
			printf("\t请输入新的城市名称:");
			scanf("%s", temp);
			strcpy(temp_1, p->name);
			strcpy(p->name, temp);
			system("cls");
			printf("\n\n修改后的信息:");
			printf("\t编号\t1 号\t\t2 号\t3 号\n");
			input_city(p, choice);//输出结果	
			printf("\t保存并退出请按‘1’，放弃操作并退出请按‘2’，继续修改请按‘3’：");
			scanf("%d", &num);
			if (num == 1)
			{
				printf("\t保存成功\n\t");
				save(2);
				return Edit_city();
			}
			else if (num == 2)
			{
				strcpy(p->name, temp_1);
				printf("\t取消操作成功\n\t");
				system("pause");
				return Edit_city();
			}
			else if (num == 3)
				return Update_city(p, choice);
			break;
		case 2:
			printf("\t请输入新的城市代号:");
			scanf("%s", temp);
			strcpy(temp_1, p->code);
			strcpy(p->code, temp);
			system("cls");
			printf("\n\n修改后的信息:");
			printf("\t编号\t1 号\t\t2 号\t3 号\n");
			input_city(p, choice);//输出结果	
			printf("\t保存并退出请按‘1’，放弃操作并退出请按‘2’，继续修改请按‘3’：");
			scanf("%d", &num);
			if (num == 1)
			{
				printf("\t保存成功\n\t");
				save(2);
				return Edit_city();
			}
			else if (num == 2)
			{
				strcpy(p->code, temp_1);
				printf("\t取消操作成功\n\t");
				system("pause");
				return Edit_city();
			}
			else if (num == 3)
				return Update_city(p, choice);
			break;
		case 3:
			printf("\t请输入新的城市介绍:");
			scanf("%s", temp);
			strcpy(temp_1, p->abstract);
			strcpy(p->abstract, temp);
			system("cls");
			printf("\n\n修改后的信息:");
			printf("\t编号\t1 号\t\t2 号\t3 号\n");
			input_city(p, choice);//输出结果	
			printf("\t保存并退出请按‘1’，放弃操作并退出请按‘2’，继续修改请按‘3’：");
			scanf("%d", &num);
			if (num == 1)
			{
				printf("\t保存成功\n\t");
				save(2);
				return Edit_city();
			}
			else if (num == 2)
			{
				strcpy(p->abstract, temp_1);
				printf("\t取消操作成功\n\t");
				system("pause");
				return Edit_city();
			}
			else if (num == 3)
				return Update_city(p, choice);
			break;
		case 0:return Edit_city();
		default:
			printf("\t请按正确编号输入！\n\t");
			system("pause");
			Update_city(p, choice);
	}
}
//查询城市信息
void Search_city(int choice)
{
	void Edit_city();//子菜单
	char cityname[10];
	int num = 1;
	CityData *p = &C, *q;//p 头指针 q 指向操作元素的前一个的指针
	q = p; p = p->next;//初始化
	printf("\t请输入城市名称：");
	scanf("%s", cityname);
	while (p != NULL)
	{
		if (strcmp(p->name, cityname) != 0)
		{
			p = p->next; q = q->next; num++;
		}
		else break;
	}
	if(p==NULL)
	{
		printf("\t为找到该城市：%s\n\t",cityname);
		system("pause");
		return;
	}
	input_city(p, num);//输出结果
	if (choice == 3)
	{
		system("pause");
		return Edit_city();
	}
	else if (choice == 2)//删除城市
		Delete_city(q, p);
	else//修改城市
		Update_city(p, num);
	system("pause");
}
//城市信息管理
void Edit_city()//子菜单
{
	int choice;//存储编号
	CityData *city = &C;
	system("cls");
	printf("\t--------------------------------\n");
	printf("\t|       城市信息管理           |\n");
	printf("\t|------------------------------|\n");
	printf("\t|  功能：1、增加城市信息       |\n");
	printf("\t|  功能：2、修改城市信息       |\n");
	printf("\t|  功能：3、删除城市信息       |\n");
	printf("\t|  功能：4、查询城市信息       |\n");
	printf("\t|  功能：5、显示已有城市信息   |\n");
	printf("\t|  功能：9、返回上一层         |\n");
	printf("\t|  功能：0、退出               |\n");
	printf("\t--------------------------------\n");
	printf("\t请输入编号：");
	scanf("%d", &choice);
	switch (choice)
	{
		case 1:Insert_city(); break;//Insert_city 增加
		case 2:Search_city(1); break;//Update_city 修改
		case 3:Search_city(2); break;//Delete_city 删除
		case 4:Search_city(3); break;//查询城市信息
		case 5:Display_city(); break;//显示所有城市信息
		case 9:break;//返回上一层
		case 0:exit(1);//退出
		default:printf("\t输入编号错误，请按给定编号输入\n\t"); system("pause"); return Edit_city();//提示错误信息并返回Edit（）函数
	}
}
//订票
void Reserve(Team *T, int len)
{
	void Menu();//加载主菜单
	int temp = 1, choice, i = 1, j = -1, k;//j 替补客户数量
	char cus_name[20], cus_id[20], cus_call[12];
	Team *p = T->next;
	CusData *cus = &Cus, *rep = &Rep, *cusp;//cusp 分配空间
	//找到订票的航班
	printf("\t请输入需要订票的航班编号：");
	scanf("%d", &choice);
	if (choice>len)
	{
		printf("\t请输入正确编号！！");
		system("pause");
		return Reserve(T, len);
	}
	//找到航班
	while (temp<choice)
	{
		p = p->next; temp++;
	}
	cus->next = p->data->data.customer;//指向航班客户信息
	rep->next = p->data->data.replace;//指向替补客户信息
	//判断票数多少
	printf("\t请输入订票数量：");
	scanf("%d", &temp);
	choice = p->data->data.num - p->data->data.seat;//剩余票数
	if (choice<temp)
	{
		printf("\t剩余票数只有%d，是否先定 %d 张票，然后剩%d人排队等候？（‘1’=>是  ‘0’=>否）", choice, choice, temp - choice);
		scanf("%d", &i);//判断是否等候
		if (i != 1)
		{
			printf("\t取消订票成功\n\t");
			system("pause");
			return Menu();
		}
		k = choice;//剩余票数
	}
	else k = temp;//输入的票数
	//订票
	printf("\t请输入订票人员信息\n");
	while (i <= k)
	{
		printf("\t请输入第%d位乘客名称：", i);
		scanf("%s", cus_name);
		printf("\t请输入第%d位乘客身份证号：", i);
		scanf("%s", cus_id);
		printf("\t请输入第%d位乘客联系电话：", i);
		scanf("%s", cus_call);
		printf("\t姓名\t身份证\t\t\t\t电话\t\t座位号\t航班编号\n");
		printf("\t%s\t%s\t\t%s\t%d\t%s\n", cus_name, cus_id, cus_call, ++p->data->data.seat, p->data->data.id);
		printf("\t确认信息（1=>确认 0=>重输）");
		scanf("%d", &j);
		if (j == 1)
		{
			cusp = (CusData *)malloc(sizeof(CusData));
			strcpy(cusp->name, cus_name);
			strcpy(cusp->id, cus_id);
			strcpy(cusp->call, cus_call);
			strcpy(cusp->air_id , p->data->data.id);
			cusp->seat = p->data->data.seat;
			cusp->next = NULL;
			//找到空的客户
			while (cus->next != NULL && strcmp(cus->id, "0") != 0)
			{
				cus = cus->next;
			}
			if (strcmp(cus->id, "0") == 0)
			{
				strcpy(cus->name, cus_name);
				strcpy(cus->id, cus_id);
				strcpy(cus->call, cus_call);
				strcpy(cus->air_id,p->data->data.id);
			}
			else
			{
				cus->next = cusp;
				cus = cus->next;
			}
			i++;//人数加1
		}
		else
		{
			p->data->data.seat--;
			system("cls");
			printf("\t请重新输入!!");
		}
	}
	p->data->data.customer = Cus.next;//乘客与航班连接起来

	//替补客户储存
	if (k == choice)
	{
		temp = temp - choice;
		i = 1;
		while (i <= temp)
		{
			printf("\t请输入第%d位替补乘客名称：", i);
			scanf("%s", cus_name);
			printf("\t请输入第%d位替补乘客身份证号：", i);
			scanf("%s", cus_id);
			printf("\t请输入第%d位替补乘客联系电话：", i);
			scanf("%s", cus_call);
			printf("\t姓名\t身份证\t\t\t\t电话\t\t座位号\t航班编号\n");
			printf("\t%s\t%s\t\t%s\t%d\t%s\n", cus_name, cus_id, cus_call, 0, p->data->data.id);
			printf("\t确认信息（1=>确认 0=>重输）");
			scanf("%d", &j);
			if (j == 1)
			{
				cusp = (CusData *)malloc(sizeof(CusData));
				strcpy(cusp->name, cus_name);
				strcpy(cusp->id, cus_id);
				strcpy(cusp->call, cus_call);
				strcpy(cusp->air_id,p->data->data.id);
				cusp->seat = p->data->data.seat;
				cusp->next = NULL;
				//找到空的客户
				while (rep->next != NULL)
				{
					rep = rep->next;
				}
				rep->next = cusp;
				rep = rep->next;
				i++;
			}
			else
			{
				system("cls");
				printf("\t请重新输入!!");
			}
		}
		p->data->data.replace = Rep.next;//替补乘客与航班连接起来
	}
	//保存并退出
	printf("\t订票成功！！\n\t");
	save(1);//保存信息
}
//退票
void Abandon()
{
	int choice=89;
	char cus_id[20],air_id[5];
	LNode *Air = &L;//初始化航班信息
	CusData *cus = &Cus, *rep = &Rep,*free;//初始化乘客(cus)与替补客户(rep)信息 free 删除乘客信息
	Air = Air->next;
	//找到订票航班(airId,call)
	printf("\t请输入需退票的航班编号：");
	scanf("%s", air_id);
	printf("\t请输入您的身份证：");
	scanf("%s", cus_id);
	while(choice!=-99)
	{
		while (Air != &L && strcmp(Air->data.id,air_id)!=0)
		{
			Air = Air->next;//指向下一个乘客
		}
		if (Air == &L && choice==-99) //没有找到就退出
		{
			printf("\t没有“%s”航班\n\t", air_id);
			system("pause");
			return;
		}
		//初始化乘客与替补客户信息
		cus = &Cus;cus->next = Air->data.customer; cus = cus->next;
		rep = &Rep;rep->next = Air->data.replace; rep = rep->next;
		free=&Rep;free->next=Air->data.replace;
			//判断该航班是否有该乘客
		while (cus != NULL && strcmp(cus->id, cus_id) != 0)
		{
			cus = cus->next;//指向下一个乘客
		}
		if (cus != NULL)//找到之后操作
		{
			//输出乘客信息
			printf("\t姓名\t身份证\t\t\t\t电话\t\t座位号\t航班编号\n");
			printf("\t%s\t%s\t\t%s\t%d\t%s\n", cus->name, cus->id, cus->call, cus->seat, cus->air_id);
			printf("\t是否确认退票（‘1’=>退票  '0'=>取消操作）");
			scanf("%d", &choice);
			if (choice == 1)
			{
				//询问替补客户是否乘坐该航班
				while (rep != NULL && strcmp(rep->id,"0")!=0)
				{
					printf("\t尊敬的%s，本航班已有空位，是否需要？（‘1’=>是 ‘0’=>否）", rep->name);
					scanf("%d", &choice);
					if (choice == 1)
					{
						strcpy(cus->name, rep->name);
						strcpy(cus->id, rep->id);
						strcpy(cus->call, rep->call);
						//删除替补客户
						if(rep->next!=NULL)
						{
							free->next=rep->next;
						}
						else
						{
							if(free->next!=Air->data.replace)
							{
								free->next=NULL;
							}
							else
							{
								Air->data.replace=NULL;
							}
						}//删除结束
						printf("\t已经办理好手续，请按时登机\n\t");
						//保存数据
						save(1);
						return;
					}
					else 
					{
						free=free->next;
						rep = rep->next;
					}
				}
				strcpy(cus->id, "0");//替补客户不要或者没有替补客户
				Air->data.seat--;
				printf("\t已经退票！\n\t");
				//保存数据
				save(1);
				return;
			}
			else
			{
				printf("\t取消操作成功\n\t");
				system("pause");
				return;
			}
		}
		//乘客里面没有之后在替补客户里找
		while (rep != NULL && strcmp(rep->id, cus_id) != 0)
		{
			rep = rep->next;
			free=free->next;
		}
		if (rep != NULL)
		{
			//输出乘客信息
			printf("\t姓名\t身份证\t\t\t\t电话\t\t座位号\t航班编号\n");
			printf("\t%s\t%s\t\t%s\t%d\t%s\n", rep->name, rep->id, rep->call, rep->seat, rep->air_id);
			printf("\t是否确认退票（‘1’=>退票  '0'=>取消操作）");
			scanf("%d", &choice);
			if (choice == 1)
			{
				if(rep->next!=NULL)
				{
					free->next=rep->next;
				}
				else
				{
					if(free->next!=Air->data.replace)
					{
						free->next=NULL;
					}
					else
						Air->data.replace=NULL;
				}
				printf("\t已经退票！\n\t");		
				//保存数据
				save(1);
				return;
			}
			
		}
		if(Air!=&L) Air=Air->next;
		else choice=-99;//结束条件
	}
	printf("\t没有找到该用户订票信息\n\t");
	system("pause");
}
//主菜单
void Menu()
{
	int choice;//存储编号
	system("cls");
	printf("\t----------------------------\n");
	printf("\t|  欢迎使用航空管理系统\t  |\n");
	printf("\t|--------------------------|\n");
	printf("\t|功能：1、显示所有航班信息 |\n");
	printf("\t|功能：2、查询航班信息     |\n");
	printf("\t|功能：3、编辑航班信息     |\n");
	printf("\t|功能：4、订票业务办理     |\n");
	printf("\t|功能：5、退票业务办理     |\n");
	printf("\t|功能：6、输入航线图       |\n");
	printf("\t|功能：7、城市信息管理     |\n");
	printf("\t|功能：8、查询旅游路线信息 |\n");
	printf("\t|功能：9、查看已有航线图   |\n");
	printf("\t|功能：0、退出             |\n");
	printf("\t----------------------------\n");
	printf("\t请输入编号：");
	scanf("%d", &choice);
	switch (choice)
	{
		case 1:Display(); break;//Display 显示所有航班信息
		case 2:Search(1); system("pause");break;//Search(1) 查询单个航班信息
		case 3:Edit(); break;//Edit 编辑单个航班信息
		case 4:Search(4); break;//先找到航班后用 Reserve 订票
		case 5:Abandon(); break;//Abandon 退票
		case 6:campus = InitGraph(); system("pause"); break;//Map 输入航线图
		case 7:Edit_city(); break;//City 查询城市信息
		case 8:ShortestPath(campus); system("pause"); break;//Journey 查询旅游线路信息
		case 9:IntroduceCompus(campus); system("pause"); break;//查看已经输入的航线图
		case 0:break;//退出程序
		default:printf("\t输入编号错误，请按给定编号输入\n\t"); system("pause"); return Menu();//提示错误信息并返回Menu（）函数
	}
	if (choice != 0) Menu();//调用自己，实现循环
}
//主函数
void main()
{
	//初始化航班头结构体
	L.front = L.next = &L;
	//初始化城市头结构体
	C.next = NULL;
	//初始化可直达城市头结构体
	Cson.next = NULL;
	//初始化可乘客头结构体
	Cus.next = NULL;
	//初始化可替补乘客头结构体
	Rep.next = NULL;
	//初始化数据
	LodeData();
	//调用菜单
	Menu();
	printf("\t谢谢使用本程序，再见\n\t");
}