//以下为执行模块头文件

//include区
#pragma once
#include"public.h"
#include<iostream>
#include<fstream>
#include<cstring>


//类型定义区
enum ResultType {
	LIGHT, // 结束条件1，点亮了全部灯，干得漂亮
	LIMIT_, // 结束条件2，到达操作数上限
	DARK // 结束条件3，MAIN过程执行完毕
};

// 执行结果类型
struct Result {
	int steps; // 记录总步数
	ResultType result; // 用enum记录结束原因
};

//全局变量区
//这些全局变量也算是接口
OpSeq OP_ = {};//定义一个指令序列
Result result = {};//定义一个结果,作为返回值
string Pn[100][100];//存放CALL指令用
int light=0; //点亮的灯数

//函数声明区

void op_TL(Map& runMap);
void op_TR(Map& runMap);
void op_MOV(Map& runMap);
void op_JMP(Map& runMap);
void op_LIT(Map& runMap);
void op_CALL(Map& runMap,string op);
void call(OpType op,string main);
void operate(int i, Result& result);
void readin(ifstream& command);
void readin(ifstream& command, int i);
void auto_save();


//主要执行函数robot_run
//对外唯一函数接口,调用参数为指令文件路径const char* path

Result robot_run(const char* path) {//交互模块用于调用的函数
	//定义变量部分

	game.map_run = game.map_init;//把地图弄进来
	game.auto_save_id = 0;//重置自动保存标识(我不知道具体怎样,就随便弄了个)

	result.steps = 0;//初始化步数,为0

	int line = 0;//文件除了第一行的行数

	//下面是readin
	std::ifstream command;
	command.open(path);//打开文件
	if (!command.is_open())
		std::cout << "WARNING!" << endl;
	else {
		command >> line;//输入行数,指令文件的第一个东西
		//read in main operation line below
		command >> OP_.procs[0].count;
		readin(command);
		//过程行readin
		for (int i = 1; i < line; i++) {
			command >> OP_.procs[i].count;
			readin(command,i);
		}
		command.close();
	}//readin结束

	//下面是执行部分
	operate(0, result);
	//从main开始执行;
	//执行结束一共三种结果

	if (game.map_run.num_lights == light) {	// 结束条件1，点亮了全部灯，干得漂亮
		result.result = LIGHT;
		return result;
	}
	else if (result.steps >= game.limit) {// 结束条件2，到达操作数上限
		result.result = LIMIT_;
		return result;
	}
	else {// 结束条件3，MAIN过程执行完毕
		result.result = DARK;
		return result;
	}
	//结束咧!
}

//readin functions

void readin(ifstream& command){
	//主函数的readin,多了一个存Pn的步骤
	for (int i = 0; i < OP_.procs[0].count;i++) {
		command >> Pn[0][i];
		if (Pn[0][i] == "TL") {
			OP_.procs[0].ops[i] = TL;
			continue;
		}
		else if (Pn[0][i] == "TR") {
			OP_.procs[0].ops[i] = TR;
			continue;
		}
		else if (Pn[0][i] == "MOV") {
			OP_.procs[0].ops[i] = MOV;
			continue;
		}
		else if (Pn[0][i] == "LIT") {
			OP_.procs[0].ops[i] = LIT;
			continue;
		}
		else if (Pn[0][i] == "JMP") {
			OP_.procs[0].ops[i] = JMP;
			continue;
		}
		else OP_.procs[0].ops[i] = CALL;
	}
	return;
}

void readin(ifstream& command, int i) {//function readin Pn
	//command是文件操作,line是个数,OP是序列对象
		/*command >> OP.procs[i].count;*/
	int j = 0;
	while (j < OP_.procs[i].count) {//判断并输入各个指令,把他们赋给ops
		command >> Pn[i][j];
		if (Pn[i][j] == "TL")
			OP_.procs[i].ops[j] = TL;
		else if (Pn[i][j] == "TR")
			OP_.procs[i].ops[j] = TR;
		else if (Pn[i][j] == "MOV")
			OP_.procs[i].ops[j] = MOV;
		else if (Pn[i][j] == "JMP")
			OP_.procs[i].ops[j] = JMP;
		else if (Pn[i][j] == "LIT")
			OP_.procs[i].ops[j] = LIT;
		j++;
	}
	return;
}

//Basic Operator function below

void op_TL(Map &runMap) {//function turn left.change robot direction
	if (runMap.robot.dir == RIGHT)
		runMap.robot.dir =UP;
	else if (runMap.robot.dir == UP)
		runMap.robot.dir =LEFT;
	else if (runMap.robot.dir == LEFT){
		runMap.robot.dir =DOWN;
		//cout<<1<<endl;
	}
	else if (runMap.robot.dir == DOWN){
		runMap.robot.dir = RIGHT;
		//cout<<2<<endl;
	}
}

void op_TR(Map &runMap) {//function turn right.change robot direction
	if (runMap.robot.dir == RIGHT)
		runMap.robot.dir =DOWN;
	else if (runMap.robot.dir == UP)
		runMap.robot.dir =RIGHT;
	else if (runMap.robot.dir == LEFT)
		runMap.robot.dir =UP;
	else if (runMap.robot.dir == DOWN)
		runMap.robot.dir = LEFT;
}

//注意!本坐标系是以最上方块为(零,零),方向同文档,第i行(列)坐标为i-1
//下面所有全改了，x对应列，y对应行 -lyd
void MandJ(Map &runMap) {//function move forward or jump.change position x,y.
	int x = runMap.robot.pos.x,y = runMap.robot.pos.y;
	runMap.cells[y][x].robot = false;//离开之前的地方
	if (runMap.robot.dir == RIGHT || runMap.robot.dir == LEFT){
		if (runMap.robot.dir == RIGHT)//When facing left/right,change x(column)
		{
			if (runMap.robot.pos.x < runMap.col - 1)//判断是否右出界
				runMap.robot.pos.x += 1;
			else return;//否则啥也不干
		}
		else {
			if (runMap.robot.pos.x > 0)//判断是否左出界
				runMap.robot.pos.x -= 1;
			else return;//否则啥也不干,至此关于左右方向的讨论结束
		}
	}
	else if (runMap.robot.dir == UP)//else change y(row)
	{
		if (runMap.robot.pos.y > 0)//判断是否上出界
			runMap.robot.pos.y -= 1;
		else return;//否则不能输出
	}
	else 
		if (runMap.robot.pos.y < runMap.row - 1)//判断是否下出界
			runMap.robot.pos.y += 1;
		else return;
	runMap.cells[runMap.robot.pos.y][runMap.robot.pos.x].robot = true;//改变状态,到达现在的位置
}

void op_JMP(Map &runMap) {
	int x = runMap.robot.pos.x;//记录位置
	int y = runMap.robot.pos.y;
	Direction dir = runMap.robot.dir;//当前方向,用于判断地形
	if (dir == UP)
		if (runMap.cells[y][x].height == runMap.cells[y-1][x].height + 1 || runMap.cells[y][x].height == runMap.cells[y-1][x].height - 1) {
			MandJ(runMap);
			return /*true*/;
		}
		else { std::cout << "WARNING_JMP1"<<endl; return /*false*/; }
	if (dir == DOWN)
		if (runMap.cells[y][x].height == runMap.cells[y+1][x].height + 1 || runMap.cells[y][x].height == runMap.cells[y+1][x].height - 1) {
			MandJ(runMap);
			return /*true*/;
		}
		else {std::cout << "WARNING_JMP2"<<endl; return /*false*/;}
	if (dir == RIGHT)
		if (runMap.cells[y][x].height == runMap.cells[y][x+1].height + 1 || runMap.cells[y][x].height == runMap.cells[y][x+1].height - 1) {
			MandJ(runMap);
			return /*true*/;
		}
		else { std::cout << "WARNING_JMP3"<<endl; return /*false*/; }
	if (dir == LEFT)
		if (runMap.cells[y][x].height == runMap.cells[y][x-1].height + 1 || runMap.cells[y][x].height == runMap.cells[y][x-1].height - 1) {
			MandJ(runMap);
			return /*true*/;
		}
		else { std::cout << "WARNING_JMP4"<<endl; return/* false*/; }
	return;
}   

void op_MOV(Map &runMap) {
	int x = runMap.robot.pos.x;//记录位置
	int y = runMap.robot.pos.y;
	Direction dir = runMap.robot.dir;
	if (dir == UP)//四个判断
		if (runMap.cells[y][x].height == runMap.cells[y-1][x].height) {
			MandJ(runMap);
			return/* true*/;
		}
		else { std::cout << "WARNING_MOV1"<<endl; return/* false*/; }
	if (dir == DOWN)
		if (runMap.cells[y][x].height == runMap.cells[y+1][x].height) {
			MandJ(runMap);
			return /*true*/;
		}
		else { std::cout << "WARNING_MOV2"<<endl; return /*false*/; }
	if (dir == RIGHT)
		if (runMap.cells[y][x].height == runMap.cells[y][x+1].height) {
			MandJ(runMap);
			return/* true*/;
		}
		else { std::cout << "WARNING_MOV3"<<endl; return/* false*/; }
	if (dir == LEFT)
		if (runMap.cells[y][x].height == runMap.cells[y][x-1].height) {
			MandJ(runMap);
			return /*true*/;
		}
		else { std::cout << "WARNING_MOV4"<<endl; return/* false*/; }
	return;
}

void op_LIT(Map &runMap) {
	int x = runMap.robot.pos.x, y = runMap.robot.pos.y;
	int lightID = runMap.cells[y][x].light_id;
	int tmp=runMap.num_lights;
	int flag=0;
	for(int k=0;k<tmp;k++){
		if(runMap.lights[k].pos.x==x&&runMap.lights[k].pos.y==y&&runMap.lights[k].lighten==false){
			//runMap.num_lights--;
			light++;
			runMap.lights[k].lighten=true;
			return;
		}
		else if(runMap.lights[k].pos.x==x&&runMap.lights[k].pos.y==y&&runMap.lights[k].lighten==true) //有灯但已被点亮
			flag=1;
	}
	if(lightID!=1||flag==1)
		std::cout<<"WARNING_LIT"<<endl; return;
}

void op_CALL(Map& runMap,string op) {
	if (op == "MAIN")
		operate(0, result);
	else {
		int n = int(op[1] - '0');
		operate(n, result);
	}
	return;
}

void call(OpType op,string main) {//调用终端,将基本的动作均封装,
	if (op == TL)
		op_TL(game.map_run);
	else if (op == TR)
		op_TR(game.map_run);
	else if (op == JMP)
		op_JMP(game.map_run);//还没写jmp,mov判断!!!注意!!!
	else if (op == MOV)
		op_MOV(game.map_run);
	else if (op == LIT)
		op_LIT(game.map_run);
	else if (op == CALL)
		op_CALL(game.map_run, main);
	auto_save();
	//cout<<game.map_run.robot.pos.y<<" "<<game.map_run.robot.pos.x<<endl;
}

void operate(int i,Result& result) {//执行函数,
	//从某一行的开始调用到这一行的结束,
	//i是指哪一行,默认值是0(main),1为p1,2为p2
	if (result.steps > game.limit) {//用于终止递归
		cout << "LIMIT WARNING" << endl;
		return;
	}else for (int j = 0; j < OP_.procs[i].count; j++) {//将这一行都执行完
		call(OP_.procs[i].ops[j], Pn[i][j]);
		result.steps++;//每次调用call集成函数,就是一次操作,故而+1
	}
	return;
}

//void auto_save() {}