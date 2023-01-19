#include <iostream>
#include<fstream>
#include<string.h>
#include<cmath>
#include"public.h"
using namespace std;

typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef int LONG;

#pragma pack(1)
typedef struct tagBITMAPFILEHEADER {
  WORD  bfType;
  DWORD bfSize;
  WORD  bfReserved1;
  WORD  bfReserved2;
  DWORD bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
  DWORD biSize;
  LONG  biWidth;
  LONG  biHeight;
  WORD  biPlanes;
  WORD  biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG  biXPelsPerMeter;
  LONG  biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
} BITMAPINFOHEADER;
struct Pixel{
	unsigned char b,g,r;
};
const int width=800;
const int height=400;
const int robotwidth=60;
const int robotheight=120;
const int pri_x=400;
const int pri_y=295;
const int side_length=40;
ofstream fbmp;	
ifstream frobot;
Pixel pixs[height][width];
Pixel pixrobot[robotheight][robotwidth];
void background(){
	BITMAPFILEHEADER bfh={((WORD)'M')<<8|'B',width*height*3+0x36,0,0,0x36};
	BITMAPINFOHEADER bih={0x28,width,height,1,24,0,width*height*3,0,0,0,0};
	fbmp.write((char*)&bfh,sizeof(bfh));
	fbmp.write((char*)&bih,sizeof(bih));	
	for(int i=0;i<width;i++)
		for(int j=0;j<height;j++){
		pixs[j][i].b=0;
		pixs[j][i].g=130;
		pixs[j][i].r=0;
		}
}
int xcoo_trans(int x,int y){
	int x_aft=pri_x+(x-y)*side_length*0.85;
	return x_aft;
}
int ycoo_trans(int x,int y){
	int y_aft=pri_y-0.5*side_length*(x+y);
	return y_aft;
}
void draw_up_grey(int x,int y){
	for(int i=x-side_length*0.85;i<=x;i++)
		for(int j=y-int((i-x)/1.7);j<=y+int((i-x)/1.7)+side_length;j++){
			pixs[j][i].b=150;
			pixs[j][i].g=150;
			pixs[j][i].r=150;
	}
		for(int i=x+side_length*0.85;i>x;i--)
		for(int j=y+int((i-x)/1.7);j<=y-int((i-x)/1.7)+side_length;j++){
			pixs[j][i].b=150;
			pixs[j][i].g=150;
			pixs[j][i].r=150;
	}
}
void draw_up_blue(int x,int y){
	for(int i=x-side_length*0.85;i<=x;i++)
		for(int j=y-int((i-x)/1.7);j<=y+int((i-x)/1.7)+side_length;j++){
			pixs[j][i].b=180;
			pixs[j][i].g=0;
			pixs[j][i].r=0;
	}
		for(int i=x+side_length*0.85;i>x;i--)
		for(int j=y+int((i-x)/1.7);j<=y-int((i-x)/1.7)+side_length;j++){
			pixs[j][i].b=180;
			pixs[j][i].g=0;
			pixs[j][i].r=0;
	}
}
void draw_up_yellow(int x,int y){
	for(int i=x-side_length*0.85;i<=x;i++)
		for(int j=y-int((i-x)/1.7);j<=y+int((i-x)/1.7)+side_length;j++){
			pixs[j][i].b=0;
			pixs[j][i].g=220;
			pixs[j][i].r=220;
	}
		for(int i=x+side_length*0.85;i>x;i--)
		for(int j=y+int((i-x)/1.7);j<=y-int((i-x)/1.7)+side_length;j++){
			pixs[j][i].b=0;
			pixs[j][i].g=220;
			pixs[j][i].r=220;
	}
}
void draw_sidegrey(int x,int y){
	for(int i=x-side_length*0.85;i<=x;i++)
		for(int j=y-int((i-x)/1.7);j>=y-int((i-x)/1.7)-0.2*side_length;j--){
			pixs[j][i].b=180;
			pixs[j][i].g=180;
			pixs[j][i].r=180;
		}
	for(int i=x+side_length*0.85;i>x;i--)
		for(int j=y+int((i-x)/1.7);j>=y+int((i-x)/1.7)-0.2*side_length;j--){
			pixs[j][i].b=120;
			pixs[j][i].g=120;
			pixs[j][i].r=120;
		}

}
void draw_sidegreen(int x,int y){
	for(int i=x-side_length*0.85;i<=x;i++)
		for(int j=y-int((i-x)/1.7);j>=y-int((i-x)/1.7)-0.2*side_length;j--){
			pixs[j][i].b=0;
			pixs[j][i].g=130;
			pixs[j][i].r=0;
		}
	for(int i=x+side_length*0.85;i>x;i--)
		for(int j=y+int((i-x)/1.7);j>=y+int((i-x)/1.7)-0.2*side_length;j--){
			pixs[j][i].b=0;
			pixs[j][i].g=130;
			pixs[j][i].r=0;
		}

}
void draw_line(int x,int y){
	for(int i=x+side_length*0.85;i>x;i--)
		for(int j=y+int((i-x)/1.7);j>=y+int((i-x)/1.7)-1;j--){
			pixs[j][i].b=0;
			pixs[j][i].g=0;
			pixs[j][i].r=0;
	}
	for(int i=x-side_length*0.85;i<=x;i++)
		for(int j=y-int((i-x)/1.7);j>=y-int((i-x)/1.7)-1;j--){
			pixs[j][i].b=0;
			pixs[j][i].g=0;
			pixs[j][i].r=0;
		}
	for(int i=x+side_length*0.85;i>x;i--)
		for(int j=y-int((i-x)/1.7)+side_length;j<=y-int((i-x)/1.7)+1+side_length;j++){
			pixs[j][i].b=0;
			pixs[j][i].g=0;
			pixs[j][i].r=0;
	}
	for(int i=x-side_length*0.85;i<=x;i++)
		for(int j=y+int((i-x)/1.7)+side_length;j<=y+int((i-x)/1.7)+1+side_length;j++){
			pixs[j][i].b=0;
			pixs[j][i].g=0;
			pixs[j][i].r=0;
		}
	for(int j=y;j>=y-0.2*side_length;j--){
		pixs[j][x].b=0;
		pixs[j][x].g=0;
		pixs[j][x].r=0;
	}
	for(int j=y+0.5*side_length;j>=y+0.3*side_length;j--){
		pixs[j][x-int(0.85*side_length)].b=0;
		pixs[j][x-int(0.85*side_length)].g=0;
		pixs[j][x-int(0.85*side_length)].r=0;
	}
	for(int j=y+0.5*side_length;j>=y+0.3*side_length;j--){
		pixs[j][x+int(0.85*side_length)].b=0;
		pixs[j][x+int(0.85*side_length)].g=0;
		pixs[j][x+int(0.85*side_length)].r=0;
	}
}			
void draw_onepieceup_grey(int x,int y){
	draw_up_grey(x,y);
	draw_sidegrey(x,y);
	draw_line(x,y);
}
void draw_onepiecelow_grey(int x,int y){
	draw_up_grey(x,y);
	draw_sidegreen(x,y);
	draw_line(x,y);
}
void draw_onepieceup_blue(int x,int y){
	draw_up_blue(x,y);
	draw_sidegrey(x,y);
	draw_line(x,y);
}
void draw_onepiecelow_blue(int x,int y){
	draw_up_blue(x,y);
	draw_sidegreen(x,y);
	draw_line(x,y);
}
void draw_onepieceup_yellow(int x,int y){
	draw_up_yellow(x,y);
	draw_sidegrey(x,y);
	draw_line(x,y);
}
void draw_onepiecelow_yellow(int x,int y){
	draw_up_yellow(x,y);
	draw_sidegreen(x,y);
	draw_line(x,y);
}
void draw(int i,int j){
	int x_i=xcoo_trans(j,i);
	int y_i=ycoo_trans(j,i);
	bool lit=false;
	if(game.map_run.cells[i][j].height>0){
		if(game.map_run.cells[i][j].light_id==1){ //把不等于-1改为等于1，有灯
			for(int m=0;m<game.map_run.num_lights;m++){
				if(game.map_run.lights[m].pos.y==i&&game.map_run.lights[m].pos.x==j&&game.map_run.lights[m].lighten){
					lit=true;
					break;
				}
			}
			if(lit){
				if(game.map_run.cells[i][j].height==1){
					draw_onepiecelow_yellow(x_i,y_i);
					//cout<<"lit yellow below"<<endl;
				}
				else{
					draw_onepiecelow_grey(x_i,y_i);
					y_i+=0.2*side_length;
					for(int m=2;m<game.map_run.cells[i][j].height;m++){
					draw_onepieceup_grey(x_i,y_i);
					y_i+=0.2*side_length;
					}
					draw_onepieceup_yellow(x_i,y_i);
					//cout<<"lit yellow"<<endl;
				}
			}
			else{
				if(game.map_run.cells[i][j].height==1){
					draw_onepiecelow_blue(x_i,y_i);
				}
				else{
					draw_onepiecelow_grey(x_i,y_i);
					y_i+=0.2*side_length;
					for(int m=2;m<game.map_run.cells[i][j].height;m++){
						draw_onepieceup_grey(x_i,y_i);
						y_i+=0.2*side_length;
					}
					draw_onepieceup_blue(x_i,y_i);
				}
			}
		}
		else{
			draw_onepiecelow_grey(x_i,y_i);
			y_i+=0.2*side_length;
			for(int m=2;m<=game.map_run.cells[i][j].height;m++){
				draw_onepieceup_grey(x_i,y_i);
				y_i+=0.2*side_length;
			}
		}
	}
}
void draw_robot(){
	frobot.open("robot.bmp",ios::binary);
	BITMAPFILEHEADER brobot;
	BITMAPINFOHEADER irobot;
	frobot.read((char*)&brobot,sizeof(BITMAPFILEHEADER));
	frobot.read((char*)&irobot,sizeof(BITMAPINFOHEADER));
	for(int i=0;i<robotheight;i++)
		for(int j=0;j<robotwidth;j++){
			frobot.read((char*)&pixrobot[i][j],sizeof(Pixel));
		}
	frobot.close();
	int x=xcoo_trans(game.map_run.robot.pos.x,game.map_run.robot.pos.y)-0.25*robotwidth;
	int y=ycoo_trans(game.map_run.robot.pos.x,game.map_run.robot.pos.y)+0.2*side_length*(game.map_run.cells[game.map_run.robot.pos.y][game.map_run.robot.pos.x].height+1);
	if(game.map_run.robot.dir==LEFT){
		for(int i=y;i<y+robotwidth;i++)
			for(int j=x;j<x+robotwidth*0.5;j++){
				if(pixrobot[i+robotwidth-y][j-x].b==164&&pixrobot[i+robotwidth-y][j-x].g==73&&pixrobot[i+robotwidth-y][j-x].r==163){
					continue;
				}
				else{
					pixs[i][j]=pixrobot[i+robotwidth-y][j-x];
				}
			}
	}
	if(game.map_run.robot.dir==RIGHT){
		for(int i=y;i<y+robotwidth;i++)
			for(int j=x;j<x+robotwidth*0.5;j++){
				if(pixrobot[i-y][j-x+int(robotwidth*0.5)].b==164&&pixrobot[i-y][j-x+int(robotwidth*0.5)].g==73&&pixrobot[i-y][j-x+int(robotwidth*0.5)].r==163){
					continue;
				}
				else{
					pixs[i][j]=pixrobot[i-y][j-x+int(robotwidth*0.5)];
				}
			}
	}
	if(game.map_run.robot.dir==UP){
		for(int i=y;i<y+robotwidth;i++)
			for(int j=x;j<x+robotwidth*0.5;j++){
				if(pixrobot[i+robotwidth-y][j-x+int(robotwidth*0.5)].b==164&&pixrobot[i+robotwidth-y][j-x+int(robotwidth*0.5)].g==73&&pixrobot[i+robotwidth-y][j-x+int(robotwidth*0.5)].r==163){
					continue;
				}
				else{
					pixs[i][j]=pixrobot[i+robotwidth-y][j-x+int(robotwidth*0.5)];
				}
			}
	}
	if(game.map_run.robot.dir==DOWN){
		for(int i=y;i<y+robotwidth;i++)
			for(int j=x;j<x+robotwidth*0.5;j++){
				if(pixrobot[i-y][j-x].b==164&&pixrobot[i-y][j-x].g==73&&pixrobot[i-y][j-x].r==163){
					continue;
				}
				else{
					pixs[i][j]=pixrobot[i-y][j-x];
				}
			}
	}
}
void save(const char *path){
	fbmp.open((char*)path,ios::binary);
	background();
	for(int i=0;i<game.map_run.row;i++)
		for(int j=0;j<game.map_run.col;j++){
			draw(i,j);
		}
	draw_robot();
	for(int j=0;j<height;j++)
		for(int i=0;i<width;i++)
			fbmp.write((char*)&pixs[j][i],sizeof(Pixel));
	fbmp.close();
}
void auto_save(){
	char rem[MAX_PATH_LEN];
	if(game.save_path!=NULL&&strlen(game.save_path)!=0){
	int autosaveid=game.auto_save_id;
	int digit=0;
	for(int i=0;i<strlen(game.save_path);i++){
		if(game.save_path[i]=='%'){
			for(int u=2;u<strlen(game.save_path)-i;u++){
				if(game.save_path[u+i]=='d'){		
					for(int w=u+i-1;w>=i+1;w--){
						digit+=(game.save_path[w]-'0')*pow(10,u+i-1-w);
					}
					break;
				}
			}
		for(int j=0;j<=i-1;j++){
				rem[j]=game.save_path[j];
			}
		if(int(log10(autosaveid))<=digit-1){
			for(int j=digit;j>=1;j--){
				rem[i+j-1]=char(autosaveid%10+'0');
				autosaveid/=10;
			}
			for(int j=i+digit;j<strlen(game.save_path)-3-int(log10(digit))+digit;j++){
				rem[j]=game.save_path[j-digit+3+int(log10(digit))];
			}
		}
		else{
			for(int j=int(log10(autosaveid))+1;j>=1;j--){
				rem[i+j-1]=char(autosaveid%10+'0');
				autosaveid/=10;
			}
			for(int j=i+int(log10(game.auto_save_id))+1;j<(strlen(game.save_path)-2-int(log10(digit))+int(log10(game.auto_save_id)));j++){
				rem[j]=game.save_path[j+2+int(log10(digit))-int(log10(game.auto_save_id))];}
		}
		break;	
		}
	}
	save((const char*)rem);
	game.auto_save_id++;
	}
}
