#include<mouse.h>
#include<string.h>
#include<graphics.h>

class Boton{
  int xi, yi, xf, yf, estado;
  char texto[30];
  void Btextd(void);
  void Btextf(void);
  void Bfondo(void);
public:
  void Cboton(int,int,int,int,char *);
  void Bdown(void);
  void Bup(void);
  int MouseinB(void);
  void Btext(char *); 
  Boton(){}
  ~Boton(){}
};

void Boton::Cboton(int xi,int yi,int xf,int yf,char *caption){
  this->xi=xi;
  this->yi=yi;
  this->xf=xf;
  this->yf=yf;
  strcpy(texto,caption);
  Bup();
}

void Boton::Bdown(void){
  Mouse.Ocultar();
  Bfondo();
  setcolor(WHITE);
  rectangle(xi,yi,xf,yf);
  setcolor(DARKGRAY);
  line(xi,yi,xf,yi);
  line(xi,yi,xi,yf);
  Btextf();
  Mouse.Visualizar();
}

void Boton::Bup(void){
  Mouse.Ocultar();
  Bfondo();
  setcolor(DARKGRAY);
  rectangle(xi,yi,xf-1,yf-1);
  setcolor(WHITE);
  rectangle(xi+1,yi+1,xf,yf);
  Btextd();
  Mouse.Visualizar();
}

void Boton::Bfondo(void){
  setfillstyle(SOLID_FILL,LIGHTGRAY);
  bar(xi,yi,xf,yf);
}

int Boton::MouseinB(void){
  if(Mouse.Inbox(xi,yi,xf,yf,Mx,My)==true)
    return 1;
  return 0;
}

void Boton::Btext(char *text){
  strcpy(texto,text);
}

void Boton::Btextf(void){
  setcolor(0);
  moveto(xi+(xf-xi)/2,yi+(yf-yi)/2);
  settextjustify(CENTER_TEXT,CENTER_TEXT);
  outtextxy(xi+2+(xf-xi)/2,yi+2+(yf-yi)/2,texto);
}

void Boton::Btextd(void){
  setcolor(0);
  moveto(xi+(xf-xi)/2,yi+(yf-yi)/2);
  settextjustify(CENTER_TEXT,CENTER_TEXT);
  outtextxy(xi+1+(xf-xi)/2,yi+1+(yf-yi)/2,texto);
}

