#include"public.h"
#include<iostream>
#include<fstream>
#include<string.h>
#include"Operator.h"
#include"draw.h"
using namespace std;
//交互模块
OpSeq opseq;

void printmap(int k=0){
    int flag=0;
    Map testmap=game.map_init;
    if(k==1){
        testmap=game.map_run;
    }
    for(int i=0;i<testmap.row;i++){
            for(int j=0;j<testmap.col;j++){
                if(testmap.cells[i][j].height==0)
                    cout<<"\e[0m";
                else{
                    if(testmap.cells[i][j].robot==true) //有机器人
                    {
                        if(testmap.cells[i][j].light_id!=1) //没有灯
                            cout<< "\e[91;100;1m"<<testmap.cells[i][j].height;
                        else//有灯
                        {
                            for(int k=0;k<testmap.num_lights;k++){
                                if(testmap.lights[k].pos.x==j&&testmap.lights[k].pos.y==i&&testmap.lights[k].lighten==true){
                                    cout<< "\e[91;103;1m"<<testmap.cells[i][j].height; //有灯已点亮
                                    flag=1;
                                } 
                            }
                            if(!flag)
                                cout<< "\e[91;104;1m"<<testmap.cells[i][j].height; //有灯待点亮
                        }
                    }
                    else //无机器人
                    {
                        if(testmap.cells[i][j].light_id!=1) //没有灯
                            cout<< "\e[92;100;1m"<<testmap.cells[i][j].height;
                        else//有灯
                        {
                            for(int k=0;k<testmap.num_lights;k++){
                                if(testmap.lights[k].pos.x==j&&testmap.lights[k].pos.y==i&&testmap.lights[k].lighten==true){
                                    cout<< "\e[92;103;1m"<<testmap.cells[i][j].height; //有灯已点亮
                                    flag=1;
                                }
                                
                            }
                            if(!flag)
                                cout<< "\e[92;104;1m"<<testmap.cells[i][j].height; //有灯待点亮
                        }
                    }
                }
            }
            cout<<"\e[0m"<<endl;
        }
    cout<<"Robot is facing ";
    switch(testmap.robot.dir){
        case UP:
            cout<<"up."<<endl;
            break;
        case DOWN:
            cout<<"down."<<endl;
            break;
        case LEFT:
            cout<<"left."<<endl;
            break;
        case RIGHT:
            cout<<"right."<<endl;
            break;
    }
}

void LOAD(char s2[100]){
    //加载应该需要使用文件和其他模块的函数，待改
    memcpy(game.map_name,s2,strlen(s2));
    ifstream in(game.map_name);
    if(!in.fail()){
        while (!in.eof()) {
            in>>game.map_init.row>>game.map_init.col>>game.map_init.num_lights>>opseq.count;
            for(int i=0;i<game.map_init.row;i++){
                for(int j=0;j<game.map_init.col;j++)
                    in>>game.map_init.cells[i][j].height;
            }
            int x,y;
            //int c=0;
            for(int i=0;i<game.map_init.num_lights;i++){
                in>>x>>y;
                game.map_init.cells[y][x].light_id=1;
                game.map_init.lights[i].pos.x=x;
                game.map_init.lights[i].pos.y=y;
            }
            for(int i=0;i<opseq.count;i++)
                in>>game.map_init.op_limit[i];
            in>>game.map_init.robot.pos.x>>game.map_init.robot.pos.y;
            game.map_init.cells[game.map_init.robot.pos.y][game.map_init.robot.pos.x].robot=true;
            int tmp;
            in>>tmp;
            if(tmp==0)
                game.map_init.robot.dir=UP;
            else if(tmp==1)
                game.map_init.robot.dir=DOWN;
            else if(tmp==2)
                game.map_init.robot.dir=LEFT;
            else if(tmp==3)
                game.map_init.robot.dir=RIGHT;  
    }
    }
    else
        cout<<"fail to upload the map"<<endl;
    in.close();
}
void AUTOSAVE(char s2[100]){
    if(!strcmp(s2,"OFF")){
        game.auto_save_id=0;
        return;
    }
    else{
        //game.auto_save_id=1;
        strcpy(game.save_path,s2);
        auto_save();
    }
}
void LIMIT(int s3){
    game.limit=s3;
}
void STATUS(){
    if(game.map_name!=NULL){
        cout<<"Map Name: "<<game.map_name<<endl;
        cout<<"Autosave: "<<game.save_path<<endl; //这里可能需要修改
        cout<<"Step Limit: "<<game.limit<<endl;
        printmap();
        cout<<"Proc Limit: "<<endl;
        for(int i=0;i<opseq.count;i++)
            cout<<game.map_init.op_limit[i]<<" ";
        cout<<endl;
    }
}
void OP(char s2[100]){  
    ofstream out(s2);
    int n,flag=0;
    char op_[20];
    cin>>n;
    if(n>opseq.count) flag=1;
    out<<n<<endl;
    int m;
    for(int i=0;i<n;i++){
        cin>>m;
        if(m>game.map_init.op_limit[i]) flag=1;
        out<<m<<" ";
        for(int j=0;j<m;j++){
            cin>>op_;
            out<<op_<<" ";
        }
        out<<endl;
    }
    if(flag==1)
        cout<<"limit exceeded"<<endl;
    out.close();
}
void RUN(char s2[100]){
    //执行序列
    robot_run(s2);
    //输出结果
    switch(result.result){
        case LIGHT:
            cout<<"result: LIGHT."<<endl;
            break;
        case LIMIT_:
            cout<<"steps exceeded."<<endl;
            break;
        case DARK:
            cout<<"result: DARK."<<endl;
            break;
    }
    cout<<"Step(s) used: "<<result.steps<<endl;
    printmap(1);
    }

void EXIT(){
    cout<<"You are exiting the game"<<endl;
    return;
}

int main(){
    game.limit=100; //程序启动时，limit值设为100
    char s1[100],s2[100];
    while(cin>>s1){
    if(!strcmp(s1,"LOAD")){
        cin>>s2;
        LOAD(s2);
    }
    else if(!strcmp(s1,"AUTOSAVE")){
        cin>>s2;
        AUTOSAVE(s2);
    }
    else if(!strcmp(s1,"LIMIT")){
        int s3;
        cin>>s3;
        LIMIT(s3);
    }
    else if(!strcmp(s1,"STATUS"))
        STATUS();
    else if(!strcmp(s1,"OP")){
        cin>>s2;
        OP(s2);
    }
    else if(!strcmp(s1,"RUN")){
        cin>>s2;
        RUN(s2);
    }
    else if(!strcmp(s1,"EXIT")){
        EXIT();
        break;
    }
    else
        cout<<"please enter the correct command"<<endl;
    }
    return 0;
}