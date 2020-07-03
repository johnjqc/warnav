#include<dos.h>
#include<stdio.h>
#include<boton.h>
#include<conio.h>
#include<mouse.h>
#include<graphics.h>

int Px=72,Py=80,mat_usu1[10][10],mat_usu2[10][10],dif,tramp[10][10][2];
//Variables para disparar_pc
int dif2=0,dir_disp=1;
Boton b1,b2,b3,b4,b5;

void ModoVGA(void){
  int unidad=0,modo;
  initgraph(&unidad,&modo,"c:\\turboc3\\bgi");
  if(graphresult()!=0){ //Si genera error
    cprintf("Error en apertura del video");
    getch();
    exit(1);
  }
  setgraphmode(2);
}

void cuadro(int xi,int yi,int xf,int yf,char *text){
  char texto[1]; int i=0;
  setcolor(DARKGRAY);
  rectangle(xi,yi,xf,yf);
  setcolor(WHITE);
  rectangle(xi+1,yi+1,xf+1,yf+1);
  setcolor(LIGHTGRAY);
  bar(xi+2,yi+2,xf-1,yf-1);
  setcolor(0);
  settextjustify(LEFT_TEXT,TOP_TEXT);
  settextstyle(0,0,0);
  while(text[i]!='\0'){
    texto[0]=text[i];
    texto[1]='\0';
    outtextxy(xi+(i*8)+4,yi+4,texto);
    i++;
    delay(80);
  }
  //delete text;
}

void malla(int xi,int yi,int xf,int yf){
  int i,j;
  char str[1]; str[0]='A'; str[1]='\0';
  char num[1]; num[0]='1'; num[1]='\0';
  settextstyle(0,0,0);
  setfillstyle(1,7);
  bar(xi,yi,xf,yf);
  setcolor(15);
  rectangle(xi,yi,xf,yf);
  for(i=1;i<10;i++){
    setcolor(15);
    line(xi+(i*22),yi,xi+(i*22),yf); //Lineas Horizontales
    setcolor(0);
    outtextxy(xi+(i*22)-14,yi-11,num); num[0]+=1;
    setcolor(15);
    line(xi,yi+(i*22),xf,yi+(i*22)); //Lineas Verticales
    setcolor(0);
    outtextxy(xi-11,yi+(i*22)-14,str); str[0]+=1;
  }
  outtextxy(xi-11,yi+(i*22)-14,str);
  num[0]='1'; num[1]='0'; num[2]='\0'; outtextxy(xi+(i*22)-14,yi-11,num);
}

void vist_previa(){
  setlinestyle(SOLID_LINE,1,1); setcolor(15);
  rectangle(getmaxx()/2-50,Py+22*10+30,getmaxx()/2+50,Py+22*10+30+100);
  setcolor(DARKGRAY);
  rectangle(getmaxx()/2-49,Py+22*10+31,getmaxx()/2+51,Py+22*10+31+100);
  setcolor(0);
  settextjustify(CENTER_TEXT,TOP_TEXT);
  outtextxy(getmaxx()/2,Py+(22*10)+135,"Vista Previa");
}

class submarino{
  unsigned posx,posy,dir;   //La posicion de la figura en pantalla y direccion
  void borrar();   //Borra el submarino dibujado en pantalla
  void mascara();  //Dibuja la linea punteada del borde de la figura
  void del_masc(); //Borra la mascara de la pantalla
  void mover(int); //Mueve la mascara
  void girar(int); //Gira la mascara
  void preview();  //Muestra como quedara la figura al dibujarla
  void fijar_mat(int [10][10]);//Coloca en uno las posiciones de la matriz que correcponder a la figura
public:
  void fijar(int [10][10]);    //Realiza todos los procedimientos y validaciones para dibujar la figura
  void dibujar();  //Dibuja la figura el pantalla
  int valid_pos(int [10][10]); //Valida que no se dibuje una figura sobre otra
  int hundido(int [10][10]);
  submarino(){};
  ~submarino(){};
}usu1_s1,usu2_s1;

class portaviones{
  unsigned posx,posy,dir;   //La posicion de la figura en pantalla y direccion
  void borrar();   //Borra el submarino dibujado en pantalla
  void mascara();  //Dibuja la linea punteada del borde de la figura
  void del_masc(); //Borra la mascara de la pantalla
  void mover(int); //Mueve la mascara
  void girar(int); //Gira la mascara
  void preview();  //Muestra como quedara la figura al dibujarla
  void fijar_mat(int [10][10]);//Coloca en uno las posiciones de la matriz que correcponder a la figura
public:
  void fijar(int [10][10]);    //Realiza todos los procedimientos y validaciones para dibujar la figura
  void dibujar();  //Dibuja la figura el pantalla
  int valid_pos(int [10][10]); //Valida que no se dibuje una figura sobre otra
  int hundido(int [10][10]);
  portaviones(){};
  ~portaviones(){};
}usu1_p1,usu2_p1;

class lancha{
  int posx,posy,dir;   //La posicion de la figura en pantalla y direccion
  void borrar();   //Borra el submarino dibujado en pantalla
  void mascara();  //Dibuja la linea punteada del borde de la figura
  void del_masc(); //Borra la mascara de la pantalla
  void mover(int); //Mueve la mascara
  void girar(); //Gira la mascara
  void preview();  //Muestra como quedara la figura al dibujarla
  void fijar_mat(int [10][10]);//Coloca en uno las posiciones de la matriz que correcponder a la figura
public:
  void fijar(int [10][10]);    //Realiza todos los procedimientos y validaciones para dibujar la figura
  void dibujar();  //Dibuja la figura el pantalla
  int valid_pos(int [10][10]); //Valida que no se dibuje una figura sobre otra
  int hundido(int [10][10]);
  lancha(){}
  ~lancha(){};
}usu1_l1,usu1_l2,usu2_l1,usu2_l2;

void submarino::fijar(int mat[10][10]){
  int c=12,salir=1;
  posx=Px; posy=Py; dir=1;
  vist_previa();
  mascara(); setcolor(1); preview();
  do{
    c=getch();
    girar(c);
    if((c=='f' || c=='F') && valid_pos(mat)==0){
      setcolor(1); dibujar(); fijar_mat(mat); del_masc(); c=27; salir=0;
    }
    mover(c);
  }while(salir);
  del_masc();
}

void submarino::preview(){
  int x=posx, y=posy;
  switch(dir){
    case 1:
      posx=(getmaxx()/2)-11; posy=Py+(22*10)+31+5;
      setfillstyle(SOLID_FILL,7);
      bar(getmaxx()/2-48,Py+(22*10)+31+2,getmaxx()/2+50,Py+(22*10)+31+98);
      setcolor(1); dibujar();
      break;
    case 2:
      posx=(getmaxx()/2)-44; posy=Py+(22*10)+31+61;
      setfillstyle(SOLID_FILL,7);
      bar(getmaxx()/2-48,Py+(22*10)+31+2,getmaxx()/2+50,Py+(22*10)+31+98);
      setcolor(1); dibujar();
      break;
    case 3:
      posx=(getmaxx()/2)+11; posy=Py+(22*10)+31+(22*4)+5;
      setfillstyle(SOLID_FILL,7);
      bar(getmaxx()/2-48,Py+(22*10)+31+2,getmaxx()/2+50,Py+(22*10)+31+98);
      setcolor(1); dibujar();
      break;
    case 4:
      posx=(getmaxx()/2)+44; posy=Py+(22*10)+31+39;
      setfillstyle(SOLID_FILL,7);
      bar(getmaxx()/2-48,Py+(22*10)+31+2,getmaxx()/2+50,Py+(22*10)+31+98);
      setcolor(1); dibujar();
      break;
  }
  posx=x; posy=y;
}

void submarino::girar(int c){
  if(c=='g' || c=='G'){
    del_masc();
    dir++;
    if(dir==5) dir=1;
    switch(dir){
      case 1:
	if(posx+22<=Px+(22*10) && posy+(22*4)<=Py+(22*10))
	  mascara();
	else{ dir--; if(dir==0) dir=4; mascara(); }
	break;
      case 2:
	if(posx+(22*4)<=Px+(22*10) && posy-22>=Py)
	  mascara();
	else{ dir--; mascara(); }
	break;
      case 3:
	if(posx-22>=Px && posy-(22*4)>=Py)
	  mascara();
	else{	dir--; mascara(); }
	break;
      case 4:
	if(posx-(22*4)>=Px && posy+22<=Py+(22*10))
	  mascara();
	else{ dir--; mascara(); }
	break;
    }
    setcolor(1); preview();
  }
}

void submarino::fijar_mat(int m[10][10]){
  int x=0,y=0;
  x=x+((posx-Px)/22); y=y+((posy-Py)/22);
  setlinestyle(SOLID_LINE,1,1);
  switch(dir){
    case 1:
      m[x][y]=1; m[x][y+1]=1; m[x][y+2]=1; m[x][y+3]=1;
      break;
    case 2:
      m[x][y-1]=1; m[x+1][y-1]=1; m[x+2][y-1]=1; m[x+3][y-1]=1;
      break;
    case 3:
      m[x-1][y-1]=1; m[x-1][y-2]=1; m[x-1][y-3]=1; m[x-1][y-4]=1;
      break;
    case 4:
      m[x-1][y]=1; m[x-2][y]=1; m[x-3][y]=1; m[x-4][y]=1;
      break;
  }
}

int submarino::valid_pos(int m[10][10]){
  int x=0,y=0;
  x=x+((posx-Px)/22); y=y+((posy-Py)/22);
  switch(dir){
    case 1:
      if(m[x][y]==1 || m[x][y+1]==1 || m[x][y+2]==1 || m[x][y+3]==1)
	return 1;
      break;
    case 2:
      if(m[x][y-1]==1 || m[x+1][y-1]==1 || m[x+2][y-1]==1 || m[x+3][y-1]==1)
	return 1;
      break;
    case 3:
      if(m[x-1][y-1]==1 || m[x-1][y-2]==1 || m[x-1][y-3]==1 || m[x-1][y-4]==1)
	return 1;
      break;
    case 4:
      if(m[x-1][y]==1 || m[x-2][y]==1 || m[x-3][y]==1 || m[x-4][y]==1)
	return 1;
      break;
  }
  return 0;
}

void submarino::dibujar(){
  setcolor(1);
  switch(dir){
    case 1:
      //Dibujo
      setfillstyle(1,DARKGRAY);
      fillellipse(posx+11,posy+44,11-1,44-1);
      circle(posx+11,posy+44,4);
      ellipse(posx+11,posy+44,120,240,5,22);
      ellipse(posx+11,posy+44,305,55,5,22);
      line(posx+9,posy+63,posx+13,posy+63);
      line(posx+9,posy+25,posx+13,posy+25);
      setfillstyle(SOLID_FILL,getcolor());
      bar(posx+10,posy+22,posx+12,posy+40);
      break;
    case 2:
      //Dibujo
      setfillstyle(1,DARKGRAY);
      fillellipse(posx+44,posy-11,44-1,11-1);
      circle(posx+44,posy-11,4);
      ellipse(posx+44,posy-11,30,145,22,5);
      ellipse(posx+44,posy-11,210,336,22,5);
      line(posx+25,posy-13,posx+25,posy-9);
      line(posx+63,posy-13,posx+63,posy-9);
      setfillstyle(SOLID_FILL,getcolor());
      bar(posx+40,posy-10,posx+22,posy-12);
      break;
    case 3:
      //Dibujo
      setfillstyle(1,DARKGRAY);
      fillellipse(posx-11,posy-44,11-1,44-1);
      circle(posx-11,posy-44,4);
      ellipse(posx-11,posy-44,120,240,5,22);
      ellipse(posx-11,posy-44,305,55,5,22);
      line(posx-9,posy-63,posx-13,posy-63);
      line(posx-9,posy-25,posx-13,posy-25);
      setfillstyle(SOLID_FILL,getcolor());
      bar(posx-10,posy-22,posx-12,posy-40);
      break;
    case 4:
      //Dibujo
      setfillstyle(1,DARKGRAY);
      fillellipse(posx-44,posy+11,44-1,11-1);
      circle(posx-44,posy+11,4);
      ellipse(posx-44,posy+11,30,145,22,5);
      ellipse(posx-44,posy+11,210,336,22,5);
      line(posx-25,posy+13,posx-25,posy+9);
      line(posx-63,posy+12,posx-63,posy+9);
      setfillstyle(SOLID_FILL,getcolor());
      bar(posx-40,posy+10,posx-22,posy+12);
      break;
  }
}

void submarino::borrar(){
  int i;
  switch(dir){
    case 1:
      setfillstyle(1,7);
      bar(posx+1,posy+1,posx+21,posy+(22*4)-1);
      setcolor(15);
      setlinestyle(SOLID_LINE,1,1);
      for(i=1;i<=3;i++)
	line(posx,posy+(i*22),posx+22,posy+(i*22));
      break;
    case 2:
      setfillstyle(1,7);
      bar(posx+1,posy-21,posx+(22*4)-1,posy);
      setcolor(15);
      for(i=1;i<3;i++)
	line(posx+(i*22),posy-22,posx+(i*22),posy);
      break;
    case 3:
      setfillstyle(1,7);
      bar(posx+1,posy-1,posx+21,posy-(22*4)+1);
      setcolor(15);
      for(i=1;i<=3;i++)
	line(posx,posy-(i*22),posx+22,posy-(i*22));
      break;
    case 4:
      setfillstyle(1,7);
      bar(posx-(22*4),posy,posx,posy+22);
      setcolor(15);
      for(i=1;i<3;i++)
	line(posx-(i*22),posy,posx-(i*22),posy+22);
      break;
  }
}

void submarino::mascara(){
  setcolor(4);
  setlinestyle(DOTTED_LINE,1,1);
  switch(dir){
    case 1: rectangle(posx,posy,posx+22,posy+(22*4)); break;
    case 2: rectangle(posx,posy-22,posx+(22*4),posy); break;
    case 3: rectangle(posx-22,posy-(22*4),posx,posy); break;
    case 4: rectangle(posx-(22*4),posy,posx,posy+22); break;
  }
}

void submarino::del_masc(){
  setcolor(15);
  setlinestyle(SOLID_LINE,1,1);
  switch(dir){
    case 1: rectangle(posx,posy,posx+22,posy+(22*4)); break;
    case 2: rectangle(posx,posy-22,posx+(22*4),posy); break;
    case 3: rectangle(posx-22,posy-(22*4),posx,posy); break;
    case 4: rectangle(posx-(22*4),posy,posx,posy+22); break;
  }
}
void submarino::mover(int c){
  switch(c){
    case 72:  //Arr
      switch(dir){
	case 1:
	  if(posy-22>=Py){ del_masc(); posy-=22; mascara(); }
	  break;
	case 2:
	  if(posy-44>=Py){ del_masc(); posy-=22; mascara(); }
	  break;
	case 3:
	  if(posy-(22*4)-22>=Py){ del_masc(); posy-=22; mascara(); }
	  break;
	case 4:
	  if(posy-22>=Py){ del_masc(); posy-=22; mascara(); }
	  break;
      }
      break;
    case 80: //Abj
      switch(dir){
	case 1:
	  if(posy+(22*4)+22<=Py+22*10){ del_masc(); posy+=22; mascara(); }
	  break;
	case 2:
	  if(posy+22<=Py+22*10){ del_masc(); posy+=22; mascara(); }
	  break;
	case 3:
	  if(posy+22<=Py+22*10){ del_masc(); posy+=22; mascara(); }
	  break;
	case 4:
	  if(posy+44<=Py+22*10){ del_masc(); posy+=22; mascara(); }
	  break;
      }
      break;
    case 75: //Izq
      switch(dir){
	case 1:
	  if(posx-22>=Px){ del_masc(); posx-=22; mascara(); }
	  break;
	case 2:
	  if(posx-22>=Px){ del_masc(); posx-=22; mascara(); }
	  break;
	case 3:
	  if(posx-44>=Px){ del_masc(); posx-=22; mascara(); }
	  break;
	case 4:
	  if(posx-(22*5)>=Px){ del_masc(); posx-=22; mascara(); }
	  break;
      }
      break;
    case 77: //Der
      switch(dir){
	case 1:
	  if(posx+44<=Px+22*10){ del_masc(); posx+=22; mascara(); }
	  break;
	case 2:
	  if(posx+(22*5)<=Px+22*10){ del_masc(); posx+=22; mascara(); }
	  break;
	case 3:
	  if(posx+22<=Px+22*10){ del_masc(); posx+=22; mascara(); }
	  break;
	case 4:
	  if(posx+22<=Px+22*10){ del_masc(); posx+=22; mascara(); }
	  break;
      }
      break;
  }
}

int submarino::hundido(int m[10][10]){
  int x=posx,y=posy;

  x=((posx-Px)/22);
  y=((y-Py)/22);

  switch(dir){
  case 1:
    if(m[x][y]==2 && m[x][y+1]==2 && m[x][y+2]==2 && m[x][y+3]==2)
      return 1;
    break;
  case 2:
    if(m[x][y-1]==2 && m[x+1][y-1]==2 && m[x+2][y-1]==2 && m[x+3][y-1]==2)
      return 1;
    break;
  case 3:
    if(m[x-1][y-1]==2 && m[x-1][y-2]==2 && m[x-1][y-3]==2 && m[x-1][y-4]==2)
      return 1;
    break;
  case 4:
    if(m[x-1][y]==2 && m[x-2][y]==2 && m[x-3][y]==2 && m[x-4][y]==2)
      return 1;
    break;
  }
  return 0;
}

void portaviones::fijar(int mat[10][10]){
  int c=12,salir=1;
  posx=Px; posy=Py; dir=1;
  vist_previa();
  mascara(); setcolor(LIGHTGREEN); preview();
  do{
    c=getch();
    girar(c);
    if((c=='f' || c=='F') && valid_pos(mat)==0){
      dibujar(); fijar_mat(mat); del_masc(); c=27; salir=0;
    }
    mover(c);
  }while(salir);
  del_masc();
}

void portaviones::dibujar(){
  int poly[16];
  setcolor(LIGHTGREEN);
  switch(dir){
  case 1:
    poly[0]=posx+21; poly[1]=posy+23;
    poly[2]=posx+43; poly[3]=posy+23;
    poly[4]=posx+43; poly[5]=posy+23;
    poly[6]=posx+35; poly[7]=posy+44;
    poly[8]=posx+35; poly[9]=posy+44;
    poly[10]=posx+43; poly[11]=posy+65;
    poly[12]=posx+43; poly[13]=posy+65;
    poly[14]=posx+21; poly[15]=posy+65;
    setlinestyle(SOLID_LINE,1,1);
    setfillstyle(SOLID_FILL,DARKGRAY);
    sector(posx+11,posy+22,0,180,10,21);
    sector(posx+11,posy+66,180,360,10,21);
    bar(posx+1,posy+22,posx+21,posy+66);
    sector(posx+11,posy+66,180,360,8,19);
    sector(posx+11,posy+22,0,180,8,19);
    circle(posx+11,posy+(22*4)-11,4);
    rectangle(posx+9,posy+9,posx+13,posy+13);
    setcolor(getcolor());
    line(posx+1,posy+22,posx+1,posy+66);
    fillpoly(8,poly);
    setlinestyle(DOTTED_LINE,1,1);
    setcolor(15);
    line(posx+35,posy+44,posx+26,posy+(22*3));
    line(posx+44-9,posy+23,posx+22,posy+66-14);
    //Avion
    fillellipse(posx+10,posy+22+8,5,1);
    setlinestyle(SOLID_LINE,1,1);
    line(posx+10,posy+22+3,posx+11,posy+22+7);
    line(posx+10,posy+22+3,posx+10,posy+22+7);
    line(posx+11,posy+22+9,posx+10,posy+22+13);
    putpixel(posx+10,posy+22+10,getcolor());
    putpixel(posx+11,posy+22+11,getcolor());
    fillellipse(posx+10,posy+44,5,1);
    line(posx+10,posy+44-5,posx+11,posy+44-1);
    line(posx+10,posy+44-5,posx+10,posy+44-1);
    line(posx+11,posy+44+1,posx+10,posy+44+5);
    putpixel(posx+10,posy+44+2,getcolor());
    putpixel(posx+11,posy+44+3,getcolor());
    fillellipse(posx+10,posy+66-8,5,1);
    line(posx+11,posy+66-9,posx+10,posy+66-13);
    line(posx+10,posy+66-9,posx+10,posy+66-13);
    line(posx+11,posy+66-7,posx+10,posy+66-3);
    putpixel(posx+10,posy+66-6,getcolor());
    putpixel(posx+11,posy+66-5,getcolor());
    setlinestyle(DOTTED_LINE,1,1);
    setcolor(YELLOW);
    line(posx+44-5,posy+23,posx+21,posy+66);
    setcolor(DARKGRAY); setlinestyle(SOLID_LINE,1,1);
    line(posx+21,posy+23,posx+21,posy+65);
    break;
  case 2:
    poly[0]=posx+23; poly[1]=posy-21;
    poly[2]=posx+23; poly[3]=posy-43;
    poly[4]=posx+23; poly[5]=posy-43;
    poly[6]=posx+44; poly[7]=posy-35;
    poly[8]=posx+44; poly[9]=posy-35;
    poly[10]=posx+65; poly[11]=posy-43;
    poly[12]=posx+65; poly[13]=posy-43;
    poly[14]=posx+65; poly[15]=posy-21;
    setlinestyle(SOLID_LINE,1,1);
    setfillstyle(SOLID_FILL,DARKGRAY);
    sector(posx+22,posy-11,270,90,21,10);
    sector(posx+66,posy-11,0,360,21,10);
    sector(posx+22,posy-11,90,270,19,8);
    sector(posx+66,posy-11,0,360,19,8);
    bar(posx+22,posy-21,posx+66,posy-1);
    line(posx+22,posy-3,posx+22,posy-19);
    line(posx+66,posy-3,posx+66,posy-19);
    circle(posx+77,posy-11,4);
    rectangle(posx+9,posy-9,posx+13,posy-13);
    setcolor(getcolor());
    line(posx+22,posy-1,posx+66,posy-1);
    fillpoly(8,poly);
    setlinestyle(DOTTED_LINE,1,1);
    setcolor(15);
    line(posx+22,posy-35,posx+66-9,posy-22);
    line(posx+22,posy-44,posx+66,posy-22-4);
    //Avion
    fillellipse(posx+22+8,posy-10,1,5);  //1
    setlinestyle(SOLID_LINE,1,1);
    line(posx+22+3,posy-10,posx+22+7,posy-10);
    line(posx+22+3,posy-10,posx+22+7,posy-11);
    line(posx+22+9,posy-11,posx+22+13,posy-10);
    putpixel(posx+22+5,posy-11,getcolor());
    putpixel(posx+22+10,posy-10,getcolor());
    putpixel(posx+22+11,posy-11,getcolor());
    fillellipse(posx+44,posy-10,1,5);  //2
    line(posx+44-5,posy-10,posx+44-1,posy-11);
    line(posx+44+1,posy-11,posx+44+5,posy-10);
    line(posx+44-3,posy-11,posx+44-2,posy-10);
    line(posx+44+2,posy-10,posx+44+3,posy-11);
    fillellipse(posx+66-8,posy-10,1,5); //3
    line(posx+66-13,posy-10,posx+66-9,posy-11);
    line(posx+66-7,posy-11,posx+66-3,posy-10);
    line(posx+66-7,posy-10,posx+66-3,posy-10);
    putpixel(posx+66-5,posy-11,getcolor());
    putpixel(posx+66-10,posy-10,getcolor());
    putpixel(posx+66-11,posy-11,getcolor());
    setlinestyle(DOTTED_LINE,1,1);
    setcolor(YELLOW);
    line(posx+23,posy-44+5,posx+65,posy-23);
    setcolor(DARKGRAY); setlinestyle(SOLID_LINE,1,1);
    line(posx+66,posy-11,posx+86,posy-11);
    line(posx+23,posy-21,posx+65,posy-21);
    break;
  case 3:
    poly[0]=posx-21; poly[1]=posy-23;
    poly[2]=posx-43; poly[3]=posy-23;
    poly[4]=posx-43; poly[5]=posy-23;
    poly[6]=posx-35; poly[7]=posy-44;
    poly[8]=posx-35; poly[9]=posy-44;
    poly[10]=posx-43; poly[11]=posy-65;
    poly[12]=posx-43; poly[13]=posy-65;
    poly[14]=posx-21; poly[15]=posy-65;
    setlinestyle(SOLID_LINE,1,1);
    setfillstyle(SOLID_FILL,DARKGRAY);
    sector(posx-11,posy-22,180,360,10,21);
    sector(posx-11,posy-66,0,180,10,21);
    bar(posx-1,posy-22,posx-21,posy-66);
    sector(posx-11,posy-66,0,180,8,19);
    sector(posx-11,posy-22,180,360,8,19);
    circle(posx-11,posy-(22*4)+11,4);
    rectangle(posx-9,posy-9,posx-13,posy-13);
    setcolor(getcolor());
    line(posx-1,posy-22,posx-1,posy-66);
    fillpoly(8,poly);
    setlinestyle(DOTTED_LINE,1,1);
    setcolor(15);
    line(posx-35,posy-44,posx-26,posy-(22*3));
    line(posx-44+9,posy-23,posx-22,posy-66+14);
    //Avion
    fillellipse(posx-10,posy-22-8,5,1);
    setlinestyle(SOLID_LINE,1,1);
    line(posx-10,posy-22-3,posx-11,posy-22-7);
    line(posx-10,posy-22-3,posx-10,posy-22-7);
    line(posx-11,posy-22-9,posx-10,posy-22-13);
    putpixel(posx-10,posy-22-10,getcolor());
    putpixel(posx-11,posy-22-11,getcolor());
    fillellipse(posx-10,posy-44,5,1);
    line(posx-10,posy-44+5,posx-11,posy-44+1);
    line(posx-10,posy-44+5,posx-10,posy-44+1);
    line(posx-11,posy-44-1,posx-10,posy-44-5);
    putpixel(posx-10,posy-44-2,getcolor());
    putpixel(posx-11,posy-44-3,getcolor());
    fillellipse(posx-10,posy-66+8,5,1);
    line(posx-11,posy-66+9,posx-10,posy-66+13);
    line(posx-10,posy-66+9,posx-10,posy-66+13);
    line(posx-11,posy-66+7,posx-10,posy-66+3);
    putpixel(posx-10,posy-66+6,getcolor());
    putpixel(posx-11,posy-66+5,getcolor());
    setlinestyle(DOTTED_LINE,1,1);
    setcolor(YELLOW);
    line(posx-44+5,posy-23,posx-21,posy-66);
    setcolor(DARKGRAY); setlinestyle(SOLID_LINE,1,1);
    line(posx-21,posy-23,posx-21,posy-65);
    break;
  case 4:
    poly[0]=posx-23; poly[1]=posy+21;
    poly[2]=posx-23; poly[3]=posy+43;
    poly[4]=posx-23; poly[5]=posy+43;
    poly[6]=posx-44; poly[7]=posy+35;
    poly[8]=posx-44; poly[9]=posy+35;
    poly[10]=posx-65; poly[11]=posy+43;
    poly[12]=posx-65; poly[13]=posy+43;
    poly[14]=posx-65; poly[15]=posy+21;
    setlinestyle(SOLID_LINE,1,1);
    setfillstyle(SOLID_FILL,DARKGRAY);
    sector(posx-22,posy+11,0,360,21,10);
    sector(posx-66,posy+11,0,360,21,10);
    sector(posx-22,posy+11,0,360,19,8);
    sector(posx-66,posy+11,0,360,19,8);
    bar(posx-22,posy+21,posx-66,posy+1);
    line(posx-22,posy+3,posx-22,posy+19);
    line(posx-66,posy+3,posx-66,posy+19);
    circle(posx-77,posy+11,4);
    rectangle(posx-9,posy+9,posx-13,posy+13);
    setcolor(getcolor());
    line(posx-22,posy+1,posx-66,posy+1);
    fillpoly(8,poly);
    setlinestyle(DOTTED_LINE,1,1);
    setcolor(15);
    line(posx-22,posy+35,posx-66+9,posy+22);
    line(posx-22,posy+44,posx-66,posy+22+4);
    //Avion
    fillellipse(posx-22-8,posy+10,1,5);  //1
    setlinestyle(SOLID_LINE,1,1);
    line(posx-22-3,posy+10,posx-22-7,posy+10);
    line(posx-22-3,posy+10,posx-22-7,posy+11);
    line(posx-22-9,posy+11,posx-22-13,posy+10);
    putpixel(posx-22-5,posy+11,getcolor());
    putpixel(posx-22-10,posy+10,getcolor());
    putpixel(posx-22-11,posy+11,getcolor());
    fillellipse(posx-44,posy+10,1,5);  //2
    line(posx-44+5,posy+10,posx-44+1,posy+11);
    line(posx-44-1,posy+11,posx-44-5,posy+10);
    line(posx-44+3,posy+11,posx-44+2,posy+10);
    line(posx-44-2,posy+10,posx-44-3,posy+11);
    fillellipse(posx-66+8,posy+10,1,5); //3
    line(posx-66+13,posy+10,posx-66+9,posy+11);
    line(posx-66+7,posy+11,posx-66+3,posy+10);
    line(posx-66+7,posy+10,posx-66+3,posy+10);
    putpixel(posx-66+5,posy+11,getcolor());
    putpixel(posx-66+10,posy+10,getcolor());
    putpixel(posx-66+11,posy+11,getcolor());
    setlinestyle(DOTTED_LINE,1,1);
    setcolor(YELLOW);
    line(posx-23,posy+44-5,posx-65,posy+23);
    setcolor(DARKGRAY); setlinestyle(SOLID_LINE,1,1);
    line(posx-66,posy+11,posx-86,posy+11);
    line(posx-2,posy+11,posx-21,posy+11);
    line(posx-23,posy+21,posx-65,posy+21);
    break;
  }
}

void portaviones::mascara(){
  setcolor(4);
  setlinestyle(DOTTED_LINE,1,1);
  switch(dir){
  case 1:
    line(posx,posy,posx+22,posy);
    line(posx+22,posy,posx+22,posy+22);
    line(posx+22,posy+22,posx+44,posy+22);
    line(posx+44,posy+22,posx+44,posy+66);
    line(posx+44,posy+66,posx+22,posy+66);
    line(posx+22,posy+66,posx+22,posy+88);
    line(posx+22,posy+88,posx,posy+88);
    line(posx,posy+88,posx,posy);
    break;
  case 2:
    line(posx,posy,posx+(22*4),posy);
    line(posx+(22*4),posy,posx+(22*4),posy-22);
    line(posx+(22*4),posy-22,posx+(22*3),posy-22);
    line(posx+(22*3),posy-22,posx+(22*3),posy-44);
    line(posx+(22*3),posy-44,posx+22,posy-44);
    line(posx+22,posy-44,posx+22,posy-22);
    line(posx+22,posy-22,posx,posy-22);
    line(posx,posy-22,posx,posy);
    break;
  case 3:
    line(posx,posy,posx,posy-(22*4));
    line(posx,posy-(22*4),posx-22,posy-(22*4));
    line(posx-22,posy-(22*4),posx-22,posy-(22*3));
    line(posx-22,posy-(22*3),posx-44,posy-(22*3));
    line(posx-44,posy-(22*3),posx-44,posy-22);
    line(posx-44,posy-22,posx-22,posy-22);
    line(posx-22,posy-22,posx-22,posy);
    line(posx-22,posy,posx,posy);
    break;
  case 4:
    line(posx,posy,posx-(22*4),posy);
    line(posx-(22*4),posy,posx-(22*4),posy+22);
    line(posx-(22*4),posy+22,posx-(22*3),posy+22);
    line(posx-(22*3),posy+22,posx-(22*3),posy+44);
    line(posx-(22*3),posy+44,posx-22,posy+44);
    line(posx-22,posy+44,posx-22,posy+22);
    line(posx-22,posy+22,posx,posy+22);
    line(posx,posy+22,posx,posy);
    break;
  }
}

void portaviones::del_masc(){
  setcolor(15);
  setlinestyle(SOLID_LINE,1,1);
  switch(dir){
  case 1:
    line(posx,posy,posx+22,posy);
    line(posx+22,posy,posx+22,posy+22);
    line(posx+22,posy+22,posx+44,posy+22);
    line(posx+44,posy+22,posx+44,posy+66);
    line(posx+44,posy+66,posx+22,posy+66);
    line(posx+22,posy+66,posx+22,posy+88);
    line(posx+22,posy+88,posx,posy+88);
    line(posx,posy+88,posx,posy);
    break;
  case 2:
    line(posx,posy,posx+(22*4),posy);
    line(posx+(22*4),posy,posx+(22*4),posy-22);
    line(posx+(22*4),posy-22,posx+(22*3),posy-22);
    line(posx+(22*3),posy-22,posx+(22*3),posy-44);
    line(posx+(22*3),posy-44,posx+22,posy-44);
    line(posx+22,posy-44,posx+22,posy-22);
    line(posx+22,posy-22,posx,posy-22);
    line(posx,posy-22,posx,posy);
    break;
  case 3:
    line(posx,posy,posx,posy-(22*4));
    line(posx,posy-(22*4),posx-22,posy-(22*4));
    line(posx-22,posy-(22*4),posx-22,posy-(22*3));
    line(posx-22,posy-(22*3),posx-44,posy-(22*3));
    line(posx-44,posy-(22*3),posx-44,posy-22);
    line(posx-44,posy-22,posx-22,posy-22);
    line(posx-22,posy-22,posx-22,posy);
    line(posx-22,posy,posx,posy);
    break;
  case 4:
    line(posx,posy,posx-(22*4),posy);
    line(posx-(22*4),posy,posx-(22*4),posy+22);
    line(posx-(22*4),posy+22,posx-(22*3),posy+22);
    line(posx-(22*3),posy+22,posx-(22*3),posy+44);
    line(posx-(22*3),posy+44,posx-22,posy+44);
    line(posx-22,posy+44,posx-22,posy+22);
    line(posx-22,posy+22,posx,posy+22);
    line(posx,posy+22,posx,posy);
    break;
  }
}

void portaviones::mover(int c){
  switch(c){
    case 72:  //Arr
      switch(dir){
      case 1:
	if(posy-22>=Py){ del_masc(); posy-=22; mascara(); }
	break;
      case 2:
	if(posy-66>=Py){ del_masc(); posy-=22; mascara(); }
	break;
      case 3:
	if(posy-(22*4)-22>=Py){ del_masc(); posy-=22; mascara(); }
	break;
      case 4:
	if(posy-22>=Py){ del_masc(); posy-=22; mascara(); }
	break;
      }
      break;
    case 80: //Abj
      switch(dir){
      case 1:
	if(posy+(22*4)+22<=Py+22*10){ del_masc(); posy+=22; mascara(); }
	break;
      case 2:
	if(posy+22<=Py+22*10){ del_masc(); posy+=22; mascara(); }
	break;
      case 3:
	if(posy+22<=Py+22*10){ del_masc(); posy+=22; mascara(); }
	break;
      case 4:
	if(posy+66<=Py+22*10){ del_masc(); posy+=22; mascara(); }
	break;
      }
      break;
    case 75: //Izq
      switch(dir){
      case 1:
	if(posx-22>=Px){ del_masc(); posx-=22; mascara(); }
	break;
      case 2:
	if(posx-22>=Px){ del_masc(); posx-=22; mascara(); }
	break;
      case 3:
	if(posx-66>=Px){ del_masc(); posx-=22; mascara(); }
	break;
      case 4:
	if(posx-(22*5)>=Px){ del_masc(); posx-=22; mascara(); }
	break;
      }
      break;
    case 77: //Der
      switch(dir){
      case 1:
	if(posx+66<=Px+22*10){ del_masc(); posx+=22; mascara(); }
	break;
      case 2:
	if(posx+(22*5)<=Px+22*10){ del_masc(); posx+=22; mascara(); }
	break;
      case 3:
	if(posx+22<=Px+22*10){ del_masc(); posx+=22; mascara(); }
	break;
      case 4:
	if(posx+22<=Px+22*10){ del_masc(); posx+=22; mascara(); }
	break;
      }
      break;
  }
}

void portaviones::girar(int c){
  if(c=='g' || c=='G'){
  del_masc();
  dir++;
  if(dir==5) dir=1;
  switch(dir){
  case 1:
    if(posx+44<=Px+(22*10) && posy+(22*4)<=Py+(22*10))
      mascara();
    else{ dir--; if(dir==0) dir=4; mascara(); }
    break;
  case 2:
    if(posx+(22*4)<=Px+(22*10) && posy-44>=Py)
      mascara();
    else{ dir--; mascara(); }
    break;
  case 3:
    if(posx-44>=Px && posy-(22*4)>=Py)
      mascara();
    else{ dir--; mascara(); }
    break;
  case 4:
    if(posx-(22*4)>=Px && posy+44<=Py+(22*10))
      mascara();
    else{ dir--; mascara(); }
    break;
  }
  setcolor(LIGHTGREEN);
  preview();
  }
}

void portaviones::preview(){
  int x=posx, y=posy;
  switch(dir){
  case 1:
    posx=(getmaxx()/2)-22; posy=Py+(22*10)+31+5;
    setfillstyle(SOLID_FILL,7);
    bar(getmaxx()/2-48,Py+(22*10)+31+2,getmaxx()/2+50,Py+(22*10)+31+98);
    setcolor(getcolor()); dibujar();
    break;
  case 2:
    posx=(getmaxx()/2)-44; posy=Py+(22*10)+31+66;
    setfillstyle(SOLID_FILL,7);
    bar(getmaxx()/2-48,Py+(22*10)+31+2,getmaxx()/2+50,Py+(22*10)+31+98);
    setcolor(getcolor()); dibujar();
    break;
  case 3:
    posx=(getmaxx()/2)+22; posy=Py+(22*10)+31+(22*4)+5;
    setfillstyle(SOLID_FILL,7);
    bar(getmaxx()/2-48,Py+(22*10)+31+2,getmaxx()/2+50,Py+(22*10)+31+98);
    setcolor(getcolor()); dibujar();
    break;
  case 4:
    posx=(getmaxx()/2)+44; posy=Py+(22*10)+31+35;
    setfillstyle(SOLID_FILL,7);
    bar(getmaxx()/2-48,Py+(22*10)+31+2,getmaxx()/2+50,Py+(22*10)+31+98);
    setcolor(getcolor()); dibujar();
    break;
  }
  posx=x; posy=y;
}

void portaviones::fijar_mat(int m[10][10]){
  int x=0,y=0;
  x=x+((posx-Px)/22);
  y=y+((posy-Py)/22);
  setlinestyle(SOLID_LINE,1,1);
  switch(dir){
  case 1:
    m[x][y]=1;
    m[x][y+1]=1;
    m[x][y+2]=1;
    m[x][y+3]=1;
    m[x+1][y+1]=1;
    m[x+1][y+2]=1;
    break;
  case 2:
    m[x][y-1]=1;
    m[x+1][y-1]=1;
    m[x+2][y-1]=1;
    m[x+3][y-1]=1;
    m[x+1][y-2]=1;
    m[x+2][y-2]=1;
    break;
  case 3:
    m[x-1][y-1]=1;
    m[x-1][y-2]=1;
    m[x-1][y-3]=1;
    m[x-1][y-4]=1;
    m[x-2][y-2]=1;
    m[x-2][y-3]=1;
    break;
  case 4:
    m[x-1][y]=1;
    m[x-2][y]=1;
    m[x-3][y]=1;
    m[x-4][y]=1;
    m[x-2][y+1]=1;
    m[x-3][y+1]=1;
    break;
  }
}

int portaviones::valid_pos(int m[10][10]){
  int x=0,y=0;
  x=x+((posx-Px)/22);
  y=y+((posy-Py)/22);
  switch(dir){
  case 1:
    if(m[x][y]==1 || m[x][y+1]==1 || m[x][y+2]==1 || m[x][y+3]==1 || m[x+1][y+1]==1 || m[x+1][y+2]==1)
      return 1;
    break;
  case 2:
    if(m[x][y-1]==1 || m[x+1][y-1]==1 || m[x+2][y-1]==1 || m[x+3][y-1]==1 || m[x+1][y-1]==1 || m[x+2][y-1]==1)
      return 1;
    break;
  case 3:
    if(m[x-1][y-1]==1 || m[x-1][y-2]==1 || m[x-1][y-3]==1 || m[x-1][y-4]==1 || m[x-1][y-1]==1 || m[x-1][y-2]==1)
      return 1;
    break;
  case 4:
    if(m[x-1][y]==1 || m[x-2][y]==1 || m[x-3][y]==1 || m[x-4][y]==1 || m[x-2][y+1]==1 || m[x-1][y+1]==1)
      return 1;
    break;
  }
  return 0;
}

int portaviones::hundido(int mat[10][10]){
  int x=posx,y=posy;

  x=((posx-Px)/22);
  y=((y-Py)/22);

  switch(dir){
     case 1:
      if(mat[x][y]==2 && mat[x][y+1]==2 &&
	 mat[x][y+2]==2 && mat[x][y+3]==2 &&
	 mat[x+1][y+1]==2 && mat[x+1][y+2]==2)
	   return 1;
      break;
    case 2:
      if(mat[x][y-1]==2 && mat[x+1][y-1]==2 &&
	 mat[x+2][y-1]==2 && mat[x+3][y-1]==2 &&
	 mat[x+1][y-2]==2 && mat[x+2][y-2]==2)
	   return 1;
      break;
    case 3:
      if(mat[x-1][y-1]==2 && mat[x-1][y-2]==2 &&
	 mat[x-1][y-3]==2 && mat[x-1][y-4]==2 &&
	 mat[x-2][y-2]==2 && mat[x-2][y-3]==2)
	   return 1;
      break;
    case 4:
      if(mat[x-1][y]==2 && mat[x-2][y]==2 &&
	 mat[x-3][y]==2 && mat[x-4][y]==2 &&
	 mat[x-2][y+1]==2 && mat[x-3][y+1]==2)
	   return 1;
      break;
  }
  return 0;
}

void lancha::fijar(int mat[10][10]){
  int c=12,salir=1;
  posx=Px; posy=Py; dir=1;
  vist_previa();
  mascara(); setcolor(YELLOW); preview();
  do{
    c=getch();
    if(c=='g' || c=='G')
      girar();
    if((c=='f' || c=='F') && valid_pos(mat)==0){
      dibujar(); fijar_mat(mat); del_masc(); c=27; salir=0;
    }
    mover(c);
  }while(salir);
  del_masc();
}

void lancha::preview(){
  int x=posx, y=posy;
  switch(dir){
  case 1:
    posx=(getmaxx()/2)-11; posy=Py+(22*10)+31+5+22;
    setfillstyle(SOLID_FILL,7);
    bar(getmaxx()/2-48,Py+(22*10)+31+2,getmaxx()/2+50,Py+(22*10)+31+98);
    setcolor(YELLOW);
    dibujar();
    break;
  case 2:
    posx=(getmaxx()/2)-22; posy=Py+(22*10)+31+61;
    setfillstyle(SOLID_FILL,7);
    bar(getmaxx()/2-48,Py+(22*10)+31+2,getmaxx()/2+50,Py+(22*10)+31+98);
    setcolor(YELLOW);
    dibujar();
    break;
  case 3:
    posx=(getmaxx()/2)+11; posy=Py+(22*10)+31+(22*3)+5;
    setfillstyle(SOLID_FILL,7);
    bar(getmaxx()/2-48,Py+(22*10)+31+2,getmaxx()/2+50,Py+(22*10)+31+98);
    setcolor(YELLOW);
    dibujar();
    break;
  case 4:
    posx=(getmaxx()/2)+22; posy=Py+(22*10)+31+39;
    setfillstyle(SOLID_FILL,7);
    bar(getmaxx()/2-48,Py+(22*10)+31+2,getmaxx()/2+50,Py+(22*10)+31+98);
    setcolor(YELLOW);
    dibujar();
    break;
  }
  posx=x; posy=y;
}

void lancha::girar(){
    del_masc();
    dir++;
    if(dir==5) dir=1;
    switch(dir){
      case 1:
	if(posx+22<=Px+(22*10) && posy+(22*2)<=Py+(22*10))
	  mascara();
	else{ dir--; if(dir==0) dir=4; mascara(); }
	break;
      case 2:
	if(posx+(22*2)<=Px+(22*10) && posy-22>=Py)
	  mascara();
	else{ dir--; mascara(); }
	break;
      case 3:
	if(posx-22>=Px && posy-(22*2)>=Py)
	  mascara();
	else{ dir--; mascara(); }
	break;
      case 4:
	if(posx-(22*2)>=Px && posy+22<=Py+(22*10))
	  mascara();
	else{ dir--; mascara(); }
	break;
    }
    setcolor(1); preview();

}

void lancha::fijar_mat(int m[10][10]){
  int x=0,y=0;
  x=x+((posx-Px)/22); y=y+((posy-Py)/22);
  setlinestyle(SOLID_LINE,1,1);
  switch(dir){
  case 1:
    m[x][y]=1;
    m[x][y+1]=1;
    break;
  case 2:
    m[x][y-1]=1;
    m[x+1][y-1]=1;
    break;
  case 3:
    m[x-1][y-1]=1;
    m[x-1][y-2]=1;
    break;
  case 4:
    m[x-1][y]=1;
    m[x-2][y]=1;
    break;
  }
}

int lancha::valid_pos(int m[10][10]){
  int x=0,y=0;
  x=x+((posx-Px)/22); y=y+((posy-Py)/22);
  switch(dir){
  case 1:
    if(m[x][y]==1 || m[x][y+1]==1)
      return 1;
    break;
  case 2:
    if(m[x][y-1]==1 || m[x+1][y-1]==1)
      return 1;
    break;
  case 3:
    if(m[x-1][y-1]==1 || m[x-1][y-2]==1)
      return 1;
    break;
  case 4:
    if(m[x-1][y]==1 || m[x-2][y]==1)
      return 1;
    break;
  }
  return 0;
}

void lancha::dibujar(){
  void *p;
  setcolor(YELLOW);
  switch(dir){
  case 1:
    //Dibujo
    setfillstyle(1,DARKGRAY);
    setlinestyle(SOLID_LINE,1,0);
    sector(posx+11,posy+43,0,180,10,41);
    line(posx+11,posy+2,posx+11,posy+22);
    sector(posx+11,posy+41,0,180,8,18);
    line(posx+8,posy+25,posx+15,posy+25);
    line(posx+3,posy+35,posx+19,posy+35);
    line(posx+10,posy+37,posx+12,posy+37);
    line(posx+5,posy+31,posx+9,posy+31);
    line(posx+13,posy+31,posx+17,posy+31);
    break;
  case 2:
    //Dibujo
    setfillstyle(1,DARKGRAY);
    setlinestyle(SOLID_LINE,1,0);
    sector(posx+43,posy-11,90,270,41,10);
    sector(posx+41,posy-11,90,270,18,8);
    line(posx+2,posy-11,posx+22,posy-11);
    line(posx+25,posy-8,posx+25,posy-15);
    line(posx+35,posy-3,posx+35,posy-19);
    line(posx+37,posy-10,posx+37,posy-12);
    line(posx+31,posy-5,posx+31,posy-9);
    line(posx+31,posy-13,posx+31,posy-17);
    break;
  case 3:
    //Dibujo
    setfillstyle(1,DARKGRAY);
    setlinestyle(SOLID_LINE,1,0);
    sector(posx-11,posy-43,180,360,10,41);
    line(posx-11,posy-2,posx-11,posy-22);
    sector(posx-11,posy-41,180,360,8,18);
    line(posx-8,posy-25,posx-15,posy-25);
    line(posx-3,posy-35,posx-19,posy-35);
    line(posx-10,posy-37,posx-12,posy-37);
    line(posx-5,posy-31,posx-9,posy-31);
    line(posx-13,posy-31,posx-17,posy-31);
    break;
  case 4:
    //Dibujo
    setfillstyle(1,DARKGRAY);
    setlinestyle(SOLID_LINE,1,0);
    int n=imagesize(posx-43,posy,posx-88,posy+22);
    p=malloc(n);
    getimage(posx-43,posy,posx-88,posy+22,p);
    sector(posx-43,posy+11,0,360,43,10);
    sector(posx-41,posy+11,0,360,18,8);
    putimage(posx-88,posy,p,COPY_PUT);
    line(posx-2,posy+11,posx-22,posy+11);
    line(posx-25,posy+8,posx-25,posy+15);
    line(posx-35,posy+3,posx-35,posy+19);
    line(posx-37,posy+10,posx-37,posy+12);
    line(posx-31,posy+5,posx-31,posy+9);
    line(posx-31,posy+13,posx-31,posy+17);
    line(posx-43,posy+1,posx-43,posy+21);
    line(posx-41,posy+3,posx-41,posy+19);
    setcolor(DARKGRAY);
    line(posx-42,posy+2,posx-42,posy+20);
    line(posx-24,posy+11,posx-42,posy+11);
    putpixel(posx-25,posy+11,YELLOW);
    putpixel(posx-37,posy+11,YELLOW);
    putpixel(posx-35,posy+11,YELLOW);
    break;
  }
}

void lancha::borrar(){
  int i;
  switch(dir){
  case 1:
    setfillstyle(1,7);
    bar(posx+1,posy+1,posx+21,posy+(22*4)-1);
    setcolor(15);
    setlinestyle(SOLID_LINE,1,1);
    for(i=1;i<=3;i++)
      line(posx,posy+(i*22),posx+22,posy+(i*22));
    break;
  case 2:
    setfillstyle(1,7);
    bar(posx+1,posy-21,posx+(22*4)-1,posy);
    setcolor(15);
    for(i=1;i<3;i++)
      line(posx+(i*22),posy-22,posx+(i*22),posy);
    break;
  case 3:
    setfillstyle(1,7);
    bar(posx+1,posy-1,posx+21,posy-(22*4)+1);
    setcolor(15);
    for(i=1;i<=3;i++)
      line(posx,posy-(i*22),posx+22,posy-(i*22));
    break;
  case 4:
    setfillstyle(1,7);
    bar(posx-(22*4),posy,posx,posy+22);
    setcolor(15);
    for(i=1;i<3;i++)
      line(posx-(i*22),posy,posx-(i*22),posy+22);
    break;
  }
}

void lancha::mascara(){
  setcolor(4);
  setlinestyle(DOTTED_LINE,1,1);
  switch(dir){
    case 1: rectangle(posx,posy,posx+22,posy+(22*2)); break;
    case 2: rectangle(posx,posy-22,posx+(22*2),posy); break;
    case 3: rectangle(posx-22,posy-(22*2),posx,posy); break;
    case 4: rectangle(posx-(22*2),posy,posx,posy+22); break;
  }
}

void lancha::del_masc(){
  setcolor(15);
  setlinestyle(SOLID_LINE,1,1);
  switch(dir){
    case 1: rectangle(posx,posy,posx+22,posy+(22*2)); break;
    case 2: rectangle(posx,posy-22,posx+(22*2),posy); break;
    case 3: rectangle(posx-22,posy-(22*2),posx,posy); break;
    case 4: rectangle(posx-(22*2),posy,posx,posy+22); break;
  }
}
void lancha::mover(int c){
  switch(c){
    case 72:  //Arr
      switch(dir){
      case 1:
	if(posy-22>=Py){ del_masc(); posy-=22; mascara(); }
	break;
      case 2:
	if(posy-44>=Py){ del_masc(); posy-=22; mascara(); }
	break;
      case 3:
	if(posy-(22*2)-22>=Py){ del_masc(); posy-=22; mascara(); }
	break;
      case 4:
	if(posy-22>=Py){ del_masc(); posy-=22; mascara(); }
	break;
      }
      break;
    case 80: //Abj
      switch(dir){
      case 1:
	if(posy+(22*2)+22<=Py+22*10){ del_masc(); posy+=22; mascara(); }
	break;
      case 2:
	if(posy+22<=Py+22*10){ del_masc(); posy+=22; mascara(); }
	break;
      case 3:
	if(posy+22<=Py+22*10){ del_masc(); posy+=22; mascara(); }
	break;
      case 4:
	if(posy+(22*2)<=Py+22*10){ del_masc(); posy+=22; mascara(); }
	break;
      }
      break;
    case 75: //Izq
      switch(dir){
      case 1:
	if(posx-22>=Px){ del_masc(); posx-=22; mascara(); }
	break;
      case 2:
	if(posx-22>=Px){ del_masc(); posx-=22; mascara(); }
	break;
      case 3:
	if(posx-44>=Px){ del_masc(); posx-=22; mascara(); }
	break;
      case 4:
	if(posx-(22*3)>=Px){ del_masc(); posx-=22; mascara(); }
	break;
      }
      break;
    case 77: //Der
      switch(dir){
      case 1:
	if(posx+44<=Px+22*10){ del_masc(); posx+=22; mascara(); }
	break;
      case 2:
	if(posx+(22*3)<=Px+22*10){ del_masc(); posx+=22; mascara(); }
	break;
      case 3:
	if(posx+22<=Px+22*10){ del_masc(); posx+=22; mascara(); }
	break;
      case 4:
	if(posx+22<=Px+22*10){ del_masc(); posx+=22; mascara(); }
	break;
      }
      break;
  }
}

int lancha::hundido(int mat[10][10]){
  int x=posx,y=posy;

  x=((posx-Px)/22);
  y=((y-Py)/22);

  switch(dir){
    case 1:
      if(mat[x][y]==2 && mat[x][y+1]==2)
	return 1;
      break;
    case 2:
      if(mat[x][y-1]==2 && mat[x+1][y-1]==2)
	return 1;
      break;
    case 3:
      if(mat[x-1][y-1]==2 && mat[x-1][y-2]==2)
	return 1;
      break;
    case 4:
      if(mat[x-1][y]==2 && mat[x-2][y]==2)
	return 1;
      break;
  }
  return 0;
}

struct mira{
  unsigned int x,y;
}usu1={0,0},usu2={0,0},pc={0,0};

struct pc_posxy{
  unsigned int x,y,dir;
}pc_port,pc_sub,pc_lan1,pc_lan2;


void pc_pone_barcos(){
  int x,y,dir;
  int salir=1;
  randomize();
  dir=random(4); dir++;
  randomize(); x=random(10); y=random(10);
  //*************Porta_aviones*****************//
  switch(dir){
    case 1:
      while(x<0 || x>8){  x=random(10); }
      while(y<0 || y>6){  y=random(10); }
      break;
    case 2:
      while(x<0 || x>6){  x=random(10); }
      while(y<2 || y>10){ y=random(10); }
      break;
    case 3:
      while(x<2 || x>10){  x=random(10); }
      while(y<4 || y>10){  y=random(10); }
      break;
    case 4:
      while(x<4 || x>10){  x=random(10); }
      while(y<0 || y>8){  y=random(10); }
      break;
  }
  pc_port.x=x; pc_port.y=y; pc_port.dir=dir;
  switch(dir){
    case 1:
      mat_usu2[x][y]=1;
      mat_usu2[x][y+1]=1;
      mat_usu2[x][y+2]=1;
      mat_usu2[x][y+3]=1;
      mat_usu2[x+1][y+1]=1;
      mat_usu2[x+1][y+2]=1;
      break;
    case 2:
      mat_usu2[x][y-1]=1;
      mat_usu2[x+1][y-1]=1;
      mat_usu2[x+2][y-1]=1;
      mat_usu2[x+3][y-1]=1;
      mat_usu2[x+1][y-2]=1;
      mat_usu2[x+2][y-2]=1;
      break;
    case 3:
      mat_usu2[x-1][y-1]=1;
      mat_usu2[x-1][y-2]=1;
      mat_usu2[x-1][y-3]=1;
      mat_usu2[x-1][y-4]=1;
      mat_usu2[x-2][y-2]=1;
      mat_usu2[x-2][y-3]=1;
      break;
    case 4:
      mat_usu2[x-1][y]=1;
      mat_usu2[x-2][y]=1;
      mat_usu2[x-3][y]=1;
      mat_usu2[x-4][y]=1;
      mat_usu2[x-2][y+1]=1;
      mat_usu2[x-3][y+1]=1;
      break;
  }
  //********************Submarino************************//
  randomize();
  do{
     x=random(10); y=random(10);
    dir=random(4); dir++;
    switch(dir){
      case 1:
	if(x<0 || x>9) break;
	if(y<0 || y>6) break;
	if(mat_usu2[x][y]==1 || mat_usu2[x][y+1]==1 || mat_usu2[x][y+2]==1 || mat_usu2[x][y+3]==1)
	  break;
	else salir=0;
	break;
      case 2:
	if(x<0 || x>6) break;
	if(y<1 || y>10)break;
	if(mat_usu2[x][y-1]==1 || mat_usu2[x+1][y-1]==1 || mat_usu2[x+2][y-1]==1 || mat_usu2[x+3][y-1]==1)
	  break;
	else salir=0;
	break;
      case 3:
	if(x<1 || x>10) break;
	if(y<5 || y>10) break;
	if(mat_usu2[x-1][y-1]==1 || mat_usu2[x-1][y-2]==1 || mat_usu2[x-1][y-3]==1 || mat_usu2[x-1][y-4]==1)
	  break;
	else salir=0;
	break;
      case 4:
	if(x<4 || x>10) break;
	if(y<0 || y>9) break;
	if(mat_usu2[x-1][y]==1 || mat_usu2[x-2][y]==1 || mat_usu2[x-3][y]==1 || mat_usu2[x-4][y]==1)
	  break;
	else salir=0;
	break;
    }
  }while(salir);
  pc_sub.x=x; pc_sub.y=y; pc_sub.dir=dir;
  switch(dir){
    case 1:
      mat_usu2[x][y]=1; mat_usu2[x][y+1]=1; mat_usu2[x][y+2]=1; mat_usu2[x][y+3]=1;
      break;
    case 2:
      mat_usu2[x][y-1]=1; mat_usu2[x+1][y-1]=1; mat_usu2[x+2][y-1]=1; mat_usu2[x+3][y-1]=1;
      break;
    case 3:
      mat_usu2[x-1][y-1]=1; mat_usu2[x-1][y-2]=1; mat_usu2[x-1][y-3]=1; mat_usu2[x-1][y-4]=1;
      break;
    case 4:
      mat_usu2[x-1][y]=1; mat_usu2[x-2][y]=1; mat_usu2[x-3][y]=1; mat_usu2[x-4][y]=1;
      break;
  }
  //*************************Lancha 1*************************//
  salir=1; randomize();
  do{
   x=random(10); y=random(10);
  dir=random(4); dir++;
  switch(dir){
    case 1:
      if(x<0 || x>9) break;
      if(y<0 || y>8) break;
      if(mat_usu2[x][y]==1 || mat_usu2[x][y+1]==1)
	break;
      else salir=0;
      break;
    case 2:
      if(x<0 || x>8) break;
      if(y<1 || y>10) break;
      if(mat_usu2[x][y-1]==1 || mat_usu2[x+1][y-1]==1)
	break;
      else salir=0;
      break;
    case 3:
      if(x<1 || x>10) break;
      if(y<2 || y>10) break;
      if(mat_usu2[x-1][y-1]==1 || mat_usu2[x-1][y-2]==1)
	break;
      else salir=0;
      break;
    case 4:
      if(x<2 || x>10) break;
      if(y<0 || y>9) break;
      if(mat_usu2[x-1][y]==1 || mat_usu2[x-2][y]==1)
	break;
      else salir=0;
      break;
  }
  }while(salir);
  pc_lan1.x=x; pc_lan1.y=y; pc_lan1.dir=dir;
  switch(dir){
    case 1: mat_usu2[x][y]=1; mat_usu2[x][y+1]=1; break;
    case 2: mat_usu2[x][y-1]=1; mat_usu2[x+1][y-1]=1; break;
    case 3: mat_usu2[x-1][y-1]=1; mat_usu2[x-1][y-2]=1; break;
    case 4: mat_usu2[x-1][y]=1; mat_usu2[x-2][y]=1; break;
  }
  //*************************Lancha 2*************************//
  salir=1; randomize();
  do{
  x=random(10); y=random(10);
  dir=random(4); dir++;
  switch(dir){
    case 1:
      if(x<0 || x>9) break;
      if(y<0 || y>8) break;
      if(mat_usu2[x][y]==1 || mat_usu2[x][y+1]==1)
	break;
      else salir=0;
      break;
    case 2:
      if(x<0 || x>8) break;
      if(y<1 || y>10) break;
      if(mat_usu2[x][y-1]==1 || mat_usu2[x+1][y-1]==1)
	break;
      else salir=0;
      break;
    case 3:
      if(x<1 || x>10) break;
      if(y<2 || y>10) break;
      if(mat_usu2[x-1][y-1]==1 || mat_usu2[x-1][y-2]==1)
	break;
      else salir=0;
      break;
    case 4:
      if(x<2 || x>10) break;
      if(y<0 || y>9) break;
      if(mat_usu2[x-1][y]==1 || mat_usu2[x-2][y]==1)
	break;
      else salir=0;
      break;
  }
  }while(salir);
  pc_lan2.x=x; pc_lan2.y=y; pc_lan2.dir=dir;
  switch(dir){
    case 1: mat_usu2[x][y]=1; mat_usu2[x][y+1]=1; break;
    case 2: mat_usu2[x][y-1]=1; mat_usu2[x+1][y-1]=1; break;
    case 3: mat_usu2[x-1][y-1]=1; mat_usu2[x-1][y-2]=1; break;
    case 4: mat_usu2[x-1][y]=1; mat_usu2[x-2][y]=1; break;
  }
}

void pc_destruido(int msg[],int *muertos){
  if(msg[0]==0)
   switch(pc_port.dir){
     case 1:
      if(mat_usu2[pc_port.x][pc_port.y]==2 && mat_usu2[pc_port.x][pc_port.y+1]==2 &&
	 mat_usu2[pc_port.x][pc_port.y+2]==2 && mat_usu2[pc_port.x][pc_port.y+3]==2 &&
	 mat_usu2[pc_port.x+1][pc_port.y+1]==2 && mat_usu2[pc_port.x+1][pc_port.y+2]==2){
	  (*muertos)++; msg[0]=1;
	  cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"ME HAS DESTRUIDO EL PORTA-AVIONES");
	  delay(1000);
	  cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"");
      }
      break;
    case 2:
      if(mat_usu2[pc_port.x][pc_port.y-1]==2 && mat_usu2[pc_port.x+1][pc_port.y-1]==2 &&
	 mat_usu2[pc_port.x+2][pc_port.y-1]==2 && mat_usu2[pc_port.x+3][pc_port.y-1]==2 &&
	 mat_usu2[pc_port.x+1][pc_port.y-2]==2 && mat_usu2[pc_port.x+2][pc_port.y-2]==2){
	   (*muertos)++; msg[0]=1;
	   cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"ME HAS DESTRUIDO EL PORTA-AVIONES");
	   delay(1000);
	   cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"");
      }
      break;
    case 3:
      if(mat_usu2[pc_port.x-1][pc_port.y-1]==2 && mat_usu2[pc_port.x-1][pc_port.y-2]==2 &&
	 mat_usu2[pc_port.x-1][pc_port.y-3]==2 && mat_usu2[pc_port.x-1][pc_port.y-4]==2 &&
	 mat_usu2[pc_port.x-2][pc_port.y-2]==2 && mat_usu2[pc_port.x-2][pc_port.y-3]==2){
	   (*muertos)++; msg[0]=1;
	   cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"ME HAS DESTRUIDO EL PORTA-AVIONES");
	   delay(1000);
	   cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"");
      }
      break;
    case 4:
      if(mat_usu2[pc_port.x-1][pc_port.y]==2 && mat_usu2[pc_port.x-2][pc_port.y]==2 &&
	 mat_usu2[pc_port.x-3][pc_port.y]==2 && mat_usu2[pc_port.x-4][pc_port.y]==2 &&
	 mat_usu2[pc_port.x-2][pc_port.y+1]==2 && mat_usu2[pc_port.x-3][pc_port.y+1]==2){
	   (*muertos)++; msg[0]=1;
	   cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"ME HAS DESTRUIDO EL PORTA-AVIONES");
	   delay(1000);
	   cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"");
      }
      break;
  }
  if(msg[1]==0)
  switch(pc_sub.dir){
    case 1:
      if(mat_usu2[pc_sub.x][pc_sub.y]==2 && mat_usu2[pc_sub.x][pc_sub.y+1]==2 && mat_usu2[pc_sub.x][pc_sub.y+2]==2 && mat_usu2[pc_sub.x][pc_sub.y+3]==2){
	(*muertos)++; msg[1]=2;
	cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"ME HAS DESTRUIDO EL SUBMARINO");
	delay(1000);
	cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"");
      }
      break;
    case 2:
      if(mat_usu2[pc_sub.x][pc_sub.y-1]==2 && mat_usu2[pc_sub.x+1][pc_sub.y-1]==2 && mat_usu2[pc_sub.x+2][pc_sub.y-1]==2 && mat_usu2[pc_sub.x+3][pc_sub.y-1]==2){
	(*muertos)++; msg[1]=2;
	cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"ME HAS DESTRUIDO EL SUBMARINO");
	delay(1000);
	cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"");
      }
      break;
    case 3:
      if(mat_usu2[pc_sub.x-1][pc_sub.y-1]==2 && mat_usu2[pc_sub.x-1][pc_sub.y-2]==2 && mat_usu2[pc_sub.x-1][pc_sub.y-3]==2 && mat_usu2[pc_sub.x-1][pc_sub.y-4]==2){
	(*muertos)++; msg[1]=2;
	cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"ME HAS DESTRUIDO EL SUBMARINO");
	delay(1000);
	cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"");
      }
      break;
    case 4:
      if(mat_usu2[pc_sub.x-1][pc_sub.y]==2 && mat_usu2[pc_sub.x-2][pc_sub.y]==2 && mat_usu2[pc_sub.x-3][pc_sub.y]==2 && mat_usu2[pc_sub.x-4][pc_sub.y]==2){
	(*muertos)++; msg[1]=2;
	cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"ME HAS DESTRUIDO EL SUBMARINO");
	delay(1000);
	cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"");
      }
      break;
  }
  if(msg[2]==0)
  switch(pc_lan1.dir){
    case 1:
      if(mat_usu2[pc_lan1.x][pc_lan1.y]==2 && mat_usu2[pc_lan1.x][pc_lan1.y+1]==2){
	(*muertos)++; msg[2]=3;
	cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"ME HAS DESTRUIDO LA LANCHA N�1");
	delay(1000);
	cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"");
      }
      break;
    case 2:
      if(mat_usu2[pc_lan1.x][pc_lan1.y-1]==2 && mat_usu2[pc_lan1.x+1][pc_lan1.y-1]==2){
	(*muertos)++; msg[2]=3;
	cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"ME HAS DESTRUIDO LA LANCHA N�1");
	delay(1000);
	cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"");
      }
      break;
    case 3:
      if(mat_usu2[pc_lan1.x-1][pc_lan1.y-1]==2 && mat_usu2[pc_lan1.x-1][pc_lan1.y-2]==2){
	(*muertos)++; msg[2]=3;
	cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"ME HAS DESTRUIDO LA LANCHA N�1");
	delay(1000);
	cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"");
      }
      break;
    case 4:
      if(mat_usu2[pc_lan1.x-1][pc_lan1.y]==2 && mat_usu2[pc_lan1.x-2][pc_lan1.y]==2){
	(*muertos)++; msg[2]=3;
	cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"ME HAS DESTRUIDO LA LANCHA N�1");
	delay(1000);
	cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"");
      }
      break;
  }
  if(msg[3]==0)
  switch(pc_lan2.dir){
    case 1:
      if(mat_usu2[pc_lan2.x][pc_lan2.y]==2 && mat_usu2[pc_lan2.x][pc_lan2.y+1]==2){
	(*muertos)++; msg[3]=4;
	cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"ME HAS DESTRUIDO LA LANCHA N�2");
	delay(1000);
	cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"");
      }
      break;
    case 2:
      if(mat_usu2[pc_lan2.x][pc_lan2.y-1]==2 && mat_usu2[pc_lan2.x+1][pc_lan2.y-1]==2){
	(*muertos)++; msg[3]=4;
	cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"ME HAS DESTRUIDO LA LANCHA N�2");
	delay(1000);
	cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"");
      }
      break;
    case 3:
      if(mat_usu2[pc_lan2.x-1][pc_lan2.y-1]==2 && mat_usu2[pc_lan2.x-1][pc_lan2.y-2]==2){
	(*muertos)++; msg[3]=4;
	cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"ME HAS DESTRUIDO LA LANCHA N�2");
	delay(1000);
	cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"");
      }
      break;
    case 4:
      if(mat_usu2[pc_lan2.x-1][pc_lan2.y]==2 && mat_usu2[pc_lan2.x-2][pc_lan2.y]==2){
	(*muertos)++; msg[3]=4;
	cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"ME HAS DESTRUIDO LA LANCHA N�2");
	delay(1000);
	cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"");
      }
      break;
  }
}

void usu_destruido(int msg[],int *muertos,int m[10][10],
		   submarino s1,portaviones p1,lancha l1,lancha l2){
  if(msg[0]==0)
    if(s1.hundido(m)){
      (*muertos)++; msg[0]=1;
      cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"TE DESTRUI EL SUBMARINO");
      delay(1000);
      cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"");
    }
  if(msg[1]==0)
    if(p1.hundido(m)){
      (*muertos)++; msg[1]=1;
      cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"TE DESTRUI EL PORTA-AVIONES");
      delay(1000);
      cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"");
    }
  if(msg[2]==0)
    if(l1.hundido(m)){
      (*muertos)++; msg[2]=1;
      cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"TE DESTRUI LA LANCHA N�1");
      delay(1000);
      cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"");
    }
  if(msg[3]==0)
    if(l2.hundido(m)){
      (*muertos)++; msg[3]=1;
      cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"TE DESTRUI LA LANCHA N�2");
      delay(1000);
      cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"");
    }
}

void dib_mira(int x,int y){
  setcolor(RED);
  circle(x+11,y+11,7);
  circle(x+11,y+11,6);
  circle(x+11,y+11,3);
  line(x+11,y+2,x+11,y+9);
  line(x+2,y+11,x+9,y+11);
  line(x+11,y+13,x+11,y+20);
  line(x+13,y+11,x+20,y+11);
}

void mov_mira(void *sm,unsigned int key,unsigned *x,unsigned *y){
  if(key==80 || key==72 || key==77 || key==75)
    for(int i=500;i<900;i+=20){ sound(i); delay(3); }
  nosound();
  switch(key){
    case 80:     /* Se se mueve la mira para abajo */
	putimage(*x+1,*y+1,sm,COPY_PUT);
	if(*y+22<=Py+(22*9)) *y+=22;
	getimage(*x+1,*y+1,*x+21,*y+21,sm);
	dib_mira(*x,*y);
	break;
    case 72:  /* Si se mueve la mira para arriba */
	putimage(*x+1,*y+1,sm,COPY_PUT);
	if(*y-22>=Py) *y-=22;
	getimage(*x+1,*y+1,*x+21,*y+21,sm);
	dib_mira(*x,*y);
	break;
    case 77:     /*  Si se mueve la mira para la derecha  */
	putimage(*x+1,*y+1,sm,COPY_PUT);
	if(*x+22<= getmaxx()-Px-22) *x+=22;
	getimage(*x+1,*y+1,*x+21,*y+21,sm);
	dib_mira(*x,*y);
	break;
    case 75:    /* Si se mueve la mira para la izquierda  */
	putimage(*x+1,*y+1,sm,COPY_PUT);
	if(*x-22>=getmaxx()-Px-(22*10)) *x-=22;
	getimage(*x+1,*y+1,*x+21,*y+21,sm);
	dib_mira(*x,*y);
	break;
  }
}

void mov_mira1(void *sm,unsigned int key,unsigned *x,unsigned *y){
  if(key==80 || key==72 || key==77 || key==75)
    for(int i=500;i<900;i+=20){ sound(i); delay(3); }
  nosound();
  switch(key){
    case 80:     /* Se se mueve la mira para abajo */
	putimage(*x+1,*y+1,sm,COPY_PUT);
	if(*y+22<=Py+(22*9)) *y+=22;
	getimage(*x+1,*y+1,*x+21,*y+21,sm);
	dib_mira(*x,*y);
	break;
    case 72:  /* Si se mueve la mira para arriba */
	putimage(*x+1,*y+1,sm,COPY_PUT);
	if(*y-22>=Py) *y-=22;
	getimage(*x+1,*y+1,*x+21,*y+21,sm);
	dib_mira(*x,*y);
	break;
    case 77:     /*  Si se mueve la mira para la derecha  */
	putimage(*x+1,*y+1,sm,COPY_PUT);
	if(*x+22< Px+220) *x+=22;
	getimage(*x+1,*y+1,*x+21,*y+21,sm);
	dib_mira(*x,*y);
	break;
    case 75:    /* Si se mueve la mira para la izquierda  */
	putimage(*x+1,*y+1,sm,COPY_PUT);
	if(*x-22>= Px) *x-=22;
	getimage(*x+1,*y+1,*x+21,*y+21,sm);
	dib_mira(*x,*y);
	break;
  }
}

void dib_fuego(unsigned x,unsigned y){
  int i,xi=x,yi=y;

  for(i=1000;i>1;i-=100){ sound(i); delay(50); } nosound();
  setcolor(LIGHTBLUE);
  for(i=0;i<5;i++){
    line(xi+7+i,yi-i+19,xi-i+15,yi-i+19);
    line(xi+7+i,yi+19-i,xi+4,yi+12);
    line(xi+15-i,yi+19-i,xi+18,yi+12);
    delay(20);
  }
  setcolor(YELLOW);
  for(i=0;i<6;i++){
    line(x+11-i,y+14-i/2,x+11,y+14-i-i);
    line(x+11+i,y+14-i/2,x+11,y+14-i-i);
    delay(20);
  }
  setcolor(RED);
  for(i=0;i<3;i++){
    line(x+11,y+18,x+4+i*2,y+7-i);
    line(x+11,y+18,x+18-i*2,y+7-i);
  }
  for(i=20;i<80;i+=3){ sound(i); delay(100); } nosound();
}

void disparar(unsigned x,unsigned y,int mat[10][10],void *sm){
  unsigned pmx=0,pmy=0;

  pmx=pmy=0;
  pmx=10-((getmaxx()-(x+Px))/22);
  pmy=((y-Py)/22);

  if(mat[pmx][pmy]==1){
    dib_fuego(x,y);
    getimage(x+1,y+1,x+21,y+21,sm);
    //dib_mira(x,y);
    mat[pmx][pmy]=2;
  }
  if(mat[pmx][pmy]==0){
    putimage(x+1,y+1,sm,COPY_PUT);
    for(int i=1;i<9;i+=3){
      circle(x+11,y+11,i);
      sound(100*i);
      delay(80);
    }
    getimage(x+1,y+1,x+21,y+21,sm);
    //dib_mira(x,y);
    mat[pmx][pmy]=2;
  }
  nosound();
}

void disparar1(unsigned x,unsigned y,int mat[10][10],void *sm){
  unsigned pmx=0,pmy=0;

  pmx=pmy=0;
  pmx=(x-Px)/22;
  pmy=(y-Py)/22;

  if(mat[pmx][pmy]==1){
    dib_fuego(x,y);
    getimage(x+1,y+1,x+21,y+21,sm);
    //dib_mira(x,y);
    mat[pmx][pmy]=2;
  }
  if(mat[pmx][pmy]==0){
    putimage(x+1,y+1,sm,COPY_PUT);
    for(int i=1;i<9;i+=3){
      circle(x+11,y+11,i);
      sound(100*i);
      delay(80);
    }
    getimage(x+1,y+1,x+21,y+21,sm);
    //dib_mira(x,y);
    mat[pmx][pmy]=2;
  }
  nosound();
}

void disparar_pc(void *sm,int m[10][10]){
  unsigned *pmx,*pmy,x,y;//pmx1,pmy1;
  int camb_dir=dir_disp,bandera=1;

  putimage(pc.x+1,pc.y+1,sm,COPY_PUT);
  //Selecciona la dificultad
  switch(dif){
    //Nivel facil
    case 1:
      //Dispra al azar
      randomize();
      do{
	*pmx=random(10);
	*pmy=random(10);
      }while(m[*pmx][*pmy]==2);
      break;
    //Nivel medio
    case 2:
      do{
	switch(dif2){
	  //dispara buscando al rededor de un blanco encontrado
	  case 0:
	    randomize();
	    do{
	      *pmx=random(10);
	      *pmy=random(10);
	      if((m[*pmx][*pmy]!=2 || m[*pmx][*pmy]!=0) && m[*pmx][*pmy]==1)
		dif2=1;
	    }while(m[*pmx][*pmy]==2);
	    bandera=0;
	    (unsigned int)usu2.x=*pmx;
	    (unsigned int)usu2.y=*pmy;
	    break;
	  case 1:
	    switch(dir_disp){
	      case 1:
		if((*pmx)!=9 && (m[(*pmx)+1][*pmy]!=2)){
		  (*pmx)++; bandera=0;
		  if(m[*pmx][*pmy]!=1){
		    dir_disp=2; camb_dir=1;
		  }
		}else
		  dir_disp=2;
		break;
	      case 2:
		if((*pmy)!=0 && (m[*pmx][(*pmy)-1]!=2)){
		  (*pmy)--; bandera=0;
		  if(m[*pmx][*pmy]!=1){
		    dir_disp=3; camb_dir=2;
		  }
		}else
		  dir_disp=3;
		break;
	      case 3:
		if((*pmx)!=0 && (m[(*pmx)-1][*pmy]!=2)){
		  (*pmx)--; bandera=0;
		  if(m[*pmx][*pmy]!=1){
		    dir_disp=4; camb_dir=3;
		  }
		}else
		  dir_disp=4;
		  break;
	      case 4:
		if((*pmy)!=9 && (m[*pmx][(*pmy)+1]!=2)){
		  (*pmy)++; bandera=0;
		  if(m[*pmx][*pmy]!=1){ dir_disp=1; dif2=0; }
		}else{ dir_disp=1; dif2=0; camb_dir=1; }
		break;
	    }
	  break;
	}
      }while(bandera);
      break;
    //Nivel dificil
    case 3:
      do{
	switch(dif2){
	//dispara buscando al rededor de un blanco encontrado
	  case 0:
	    randomize();
	    do{
	      *pmx=random(10);
	      *pmy=random(10);
	      if((m[*pmx][*pmy]!=2 || m[*pmx][*pmy]!=0) && m[*pmx][*pmy]==1)
		dif2=1;
	    }while(m[*pmx][*pmy]==2);
	    bandera=0;
	    (unsigned int)usu2.x=*pmx;
	    (unsigned int)usu2.y=*pmy;
	    break;
	case 1:
	  switch(dir_disp){
	    case 1:
	      if((*pmx)!=9 && (m[(*pmx)+1][*pmy]==1)){
		(*pmx)++; bandera=0; dir_disp=2;
	      }else
		dir_disp=2;
	      break;
	    case 2:
	      if((*pmy)!=0 && (m[*pmx][(*pmy)-1]==1)){
		(*pmy)--; bandera=0; dir_disp=1;
	      }else
		dir_disp=3;
	      break;
	    case 3:
	      if((*pmx)!=0 && (m[(*pmx)-1][*pmy]==1)){
		(*pmx)--; bandera=0; dir_disp=1;
	      }else
		dir_disp=4;
	      break;
	    case 4:
	      if((*pmy)!=9 && (m[*pmx][(*pmy)+1]==1)){
		(*pmy)++; bandera=0; dir_disp=1;
	      }else{ dir_disp=1; dif2=0; }
	      break;
	  }
	break;
      }
     }while(bandera);
    break;
  }
  x=Px+(22*(*pmx));
  y=Py+(22*(*pmy));

  if(m[*pmx][*pmy]==1){
    dib_fuego(x,y);
    getimage(x+1,y+1,x+21,y+21,sm);
    //dib_mira(x,y);
    m[*pmx][*pmy]=2;
  }
  if(m[*pmx][*pmy]==0){
    for(int i=1;i<9;i+=3){
      setcolor(4);
      circle(x+11,y+11,i);
      sound(100*i);
      delay(80);
    }
    getimage(x+1,y+1,x+21,y+21,sm);
    //dib_mira(x,y);
    m[*pmx][*pmy]=2;
  }
  nosound();
  pc.x=x; pc.y=y;

  if(dif==2)
  if(camb_dir!=dir_disp)
    switch(camb_dir){
      case 1:
	(*pmx)=(unsigned int)usu2.x; break;
      case 2:
	(*pmy)=(unsigned int)usu2.y; break;
      case 3:
	(*pmx)=(unsigned int)usu2.x; break;
      case 4:
	(*pmy)=(unsigned int)usu2.y; break;
    }
}

void rect_oval(int xi,int yi,int xf,int yf,int f,int r){
  setfillstyle(SOLID_FILL,f);
  bar(xi,yi+r,xf,yf-r);
  bar(xi+r,yi,xf-r,yi+r);
  bar(xi+r,yf-r,xf-r,yf);
  setcolor(f);
  fillellipse(xi+r,yi+r,r,r);
  fillellipse(xf-r,yi+r,r,r);
  fillellipse(xi+r,yf-r,r,r);
  fillellipse(xf-r,yf-r,r,r);
}


void titulo(int x,int y,int fond,int frent){
  char *nom1="GUERRA",*nom2="NAVAL",let[1];
  int i,j=0,a=12;

  do{
    let[0]=nom1[j++]; let[1]='\0';
    setcolor(fond);
    for(i=0;i<a;i++)
      outtextxy(x+2+i,y+i,let);
    setcolor(frent);
    outtextxy(x+2+i,y+i,let);
    x+=50; a-=2;
  }while(nom1[j]!='\0');
  j=0; a=4; x+=10;
  do{
    let[0]=nom2[j++]; let[1]='\0';
    setcolor(fond);
    for(i=0;i<a;i++)
      outtextxy(x-2-i,y+i,let);
    setcolor(frent);
    outtextxy(x-2-i,y+i,let);
    x+=50; a+=2;
  }while(nom2[j]!='\0');
}

int dificultad(){
  int col,x=30,y=getmaxy()-30,dir=1;
  //Dibujo fondo
  //setfillstyle(SOLID_FILL,LIGHTGRAY);
  //bar(0,0,getmaxx(),getmaxy());
  rect_oval(getmaxx()/2-120,140,getmaxx()/2+120,390,4,20);
  int num=0;
  Boton b1,b2,b3,b4;
  b1.Cboton(getmaxx()/2-50,150,getmaxx()/2+50,180,"FACIL");
  b2.Cboton(getmaxx()/2-50,200,getmaxx()/2+50,230,"MEDIO");
  b3.Cboton(getmaxx()/2-50,250,getmaxx()/2+50,280,"DIFICIL");
  b4.Cboton(getmaxx()/2-50,300,getmaxx()/2+50,330,"REGRESAR");

  do{
    E=Mouse.Evento(Mx,My);
    if(Mouse.Movido()){
      col=random(16); settextjustify(CENTER_TEXT,CENTER_TEXT);
      titulo(40,20,col,col+1);
    }
    settextjustify(LEFT_TEXT,TOP_TEXT);
    if(x<=getmaxx()-199 && dir){
      setcolor(7);
      outtextxy(x,y,"JOHN QUIROGA C");
      setcolor(1); x+=1;
      outtextxy(x,y,"JOHN QUIROGA C");
      if(x==getmaxx()-199) dir=0;
    }
    else if(x>=30){
      setcolor(7);
      outtextxy(x,y,"JOHN QUIROGA C");
      setcolor(1); x-=1;
      outtextxy(x,y,"JOHN QUIROGA C");
      if(x==30) dir=1;
    }
    if(b1.MouseinB() || b2.MouseinB() || b3.MouseinB() || b4.MouseinB())
      Mouse.FijaCursorG(HandCursor);
    else
      Mouse.FijaCursorG(ArrowCursor);
    if(E==LMouseDown || E==LMouseStillDown){
      if(b1.MouseinB()){
	b1.Bdown();
	while(b1.MouseinB() && E==LMouseStillDown) E=Mouse.Evento(Mx,My);
	b1.Bup();
	if(b1.MouseinB() && E==LMouseUp)
	  num=1;
      }
      if(b2.MouseinB()){
	b2.Bdown();
	while(b2.MouseinB() && E==LMouseStillDown) E=Mouse.Evento(Mx,My);
	b2.Bup();
	if(b2.MouseinB() && E==LMouseUp)
	  num=2;
      }
      if(b3.MouseinB()){
	b3.Bdown();
	while(b3.MouseinB() && E==LMouseStillDown) E=Mouse.Evento(Mx,My);
	b3.Bup();
	if(b3.MouseinB() && E==LMouseUp)
	  num=3;
      }
      if(b4.MouseinB()){
	b4.Bdown();
	while(b4.MouseinB() && E==LMouseStillDown) E=Mouse.Evento(Mx,My);
	b4.Bup();
	if(b4.MouseinB() && E==LMouseUp)
	  num=4;
      }
    }
  }while(num==0);
  return num;
}

void trampa(int x,int y,int tram[10][10][2],int pos){
  int i,j;
  setcolor(0);
  settextstyle(0,0,0);
  settextjustify(LEFT_TEXT,TOP_TEXT);
  outtextxy(x,y-10,"OPONENTE");
  setfillstyle(1,1);
  for(i=0;i<10;i++)
    for(j=0;j<10;j++){
      if(tram[i][j][pos]==1)
	bar(x+(6*i),y+(6*j),x+(6*i)+6,y+(6*j)+6);
      }
  rectangle(x,y,x+60,y+60);
  for(i=1;i<10;i++){
    line(x,y+(6*i),x+60,y+(6*i));
    line(x+(6*i),y,x+(6*i),y+60);
  }
  getch(); getch();
  setfillstyle(1,7);
  bar(x,y-10,x+60+3,y+60);
}

void sound_muerte(){
  //La
  sound(550);	delay(600);	nosound();
  delay(100);
  //La
  sound(550);	delay(600);	nosound();
  delay(100);
  //La
  sound(550);	delay(300);	nosound();
  //La
  sound(550);	delay(300);	nosound();
  delay(50);
  //Do
  sound(650);	delay(500);	nosound();
  //Si
  sound(600);	delay(400);	nosound();
  //Si
  sound(600);	delay(400);	nosound();
  //La
  sound(550);	delay(400);	nosound();
  //La
  sound(550);	delay(400);	nosound();
  delay(50);
  //Sol
  sound(500);	delay(600);	nosound();
  //La
  sound(550);	delay(900);	nosound();
  delay(1000);
}

void game_over(){
  rect_oval(getmaxx()/2-200,100,getmaxx()/2+200,200,3,20);
  rect_oval(getmaxx()/2-200+7,100+7,getmaxx()/2+200-7,200-7,1,15);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    settextstyle(10,0,4);
    setcolor(0);
    outtextxy(getmaxx()/2,140,"FIN DEL JUEGO");
    setcolor(4);
  outtextxy(getmaxx()/2+4,140+4,"FIN DEL JUEGO");
  sound_muerte();
}

void un_jugador(){
  int mat_pc[10][10],x,y,muertos_pc=0,muertos_usu=0,t=0,i,j;
  unsigned int n,key,bandera;
  int msg_pc[4]={0,0,0,0},msg_usu[4]={0,0,0,0};
  usu1.x=getmaxx()-Px-(22*10), usu1.y=Py;
  pc.x=Px; pc.y=Py;
  void *sm,*sm_pc;
  //Dibujo fondo
  setfillstyle(SOLID_FILL,LIGHTGRAY);
  bar(0,0,getmaxx(),getmaxy());
  //Creo la cuadricula
  malla(Px,Py,Px+(22*10),Py+(22*10));
  malla(getmaxx()-(22*10)-Px,Py,getmaxx()-Px,Py+(22*10));

  n=imagesize(Px+1,Py+1,Px+21,Py+21);
  sm=malloc(n);  sm_pc=malloc(n);
  getimage(usu1.x+1,usu1.y+1,usu1.x+21,usu1.y+21,sm);
  cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"Puedes moverlos con las flechas, girarlos con G y fijarlos con F");
  usu1_p1.fijar(mat_usu1);
  usu1_s1.fijar(mat_usu1);
  usu1_l1.fijar(mat_usu1);
  usu1_l2.fijar(mat_usu1);
  if(usu1_p1.valid_pos(mat_usu1)) usu1_p1.dibujar();
  if(usu1_s1.valid_pos(mat_usu1)) usu1_s1.dibujar();
  if(usu1_l1.valid_pos(mat_usu1)) usu1_l1.dibujar();
  if(usu1_l2.valid_pos(mat_usu1)) usu1_l2.dibujar();
  cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"");

  getimage(pc.x+1,pc.y+1,pc.x+21,pc.y+21,sm_pc);
  //Borro la vista previa
  setfillstyle(SOLID_FILL,7);
  bar(getmaxx()/2-50,Py+22*10+30,getmaxx()/2+51,Py+22*10+31+100+11);

  cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"Un momento por favor, estoy ubicando mis barcos...");
  pc_pone_barcos();

  cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"");
  for(x=0;x<10;x++)
    for(y=0;y<10;y++){
      mat_pc[x][y]=mat_usu1[x][y];
      tramp[x][y][0]=mat_usu2[x][y];
    }

  cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"Puedes disparar con D y salir con ESC");
  delay(1000);
  cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"Seleccionando quien juega primero...");
  delay(1000);
  bandera=random(2);
  if(bandera==0)
    cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"Comienza jugando el usuario");
  else
    cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"Comienza jugando el computador");
  cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"");
  do{
    switch(bandera){
      case 0:
	do{
	  dib_mira(usu1.x,usu1.y);
	  key=getch();
	  if(key==27){ game_over(); break; }

	  if(key=='t' && t==0) t++;
	  else if(key=='r' && t==1) t++;
	  else if(key=='a' && t==2) t++;
	  else if(key=='m' && t==3) t++;
	  else if(key=='p' && t==4) t++;
	  else if(key=='a' && t==5) t++;
	  else t=0;
	  if(t==6){
	    trampa(getmaxx()-120,getmaxy()-120,tramp,0);
	  }
	  mov_mira(sm,key,&usu1.x,&usu1.y);
	  if(key=='d' || key=='D'){
	    putimage(usu1.x+1,usu1.y+1,sm,COPY_PUT);
	    if(mat_usu2[10-((getmaxx()-(usu1.x+Px))/22)][(usu1.y-Py)/22]!=2)
	    bandera++;
	    disparar(usu1.x,usu1.y,mat_usu2,sm);
	  }
	}while(bandera==0);
	bandera=1;
	break;
      case 1:
	disparar_pc(sm_pc,mat_pc);
	bandera=0;
	break;
    }
    if(key==27) break;
    pc_destruido(msg_pc,&muertos_pc);
    usu_destruido(msg_usu,&muertos_usu,mat_pc,usu1_s1,usu1_p1,usu1_l1,usu1_l2);
  }while(muertos_pc!=4 && muertos_usu!=4);
  if(muertos_usu==4){
    setfillstyle(1,7);
    bar(getmaxx()/2-120,getmaxy()/2-20,getmaxx()/2+120,getmaxy()/2+20);
    setcolor(8);
    rectangle(getmaxx()/2-120,getmaxy()/2-20,getmaxx()/2+120,getmaxy()/2+20);
    setcolor(15);
    rectangle(getmaxx()/2-120+1,getmaxy()/2-20+1,getmaxx()/2+121,getmaxy()/2+21);
    settextjustify(CENTER_TEXT,CENTER_TEXT); setcolor(0);
    outtextxy(getmaxx()/2,getmaxy()/2-7,"El Computador destrullo la");
    outtextxy(getmaxx()/2,getmaxy()/2+7,"flota del Usuario");
    game_over();
  }else if(muertos_pc==4){
    setfillstyle(1,7);
    bar(getmaxx()/2-120,getmaxy()/2-20,getmaxx()/2+120,getmaxy()/2+20);
    setcolor(8);
    rectangle(getmaxx()/2-120,getmaxy()/2-20,getmaxx()/2+120,getmaxy()/2+20);
    setcolor(15);
    rectangle(getmaxx()/2-120+1,getmaxy()/2-20+1,getmaxx()/2+121,getmaxy()/2+21);
    settextjustify(CENTER_TEXT,CENTER_TEXT); setcolor(0);
    outtextxy(getmaxx()/2,getmaxy()/2-7,"El Usuario destrullo la");
    outtextxy(getmaxx()/2,getmaxy()/2+7,"flota del Computador");
    game_over();
  }

  for(i=0;i<10;i++)
    for(j=0;j<10;j++)
      mat_usu1[i][j]=mat_usu2[i][j]=tramp[i][j][0]=0;
  free(sm);
}

//FUNCIONES PARA GUARDAR UNA IMAGEN EN UN ARCHIVO Y RECUPERARLA
void* capimagen(int izq,int arr,int der,int abj,unsigned *tam){
  void* imagen;
  *tam=imagesize(izq,arr,der,abj);
  imagen=malloc(*tam);
  getimage(izq,arr,der,abj,imagen);
  putimage(izq,arr,imagen,COPY_PUT);
  free(imagen);
  return(imagen);
}

void save(void* imagen,unsigned tam,char *nom){
  FILE *f1=fopen(nom,"w");
  fwrite(imagen,tam,1,f1);
  fflush(f1);
  fclose(f1);
}

void* open(char *nom){
  unsigned xdim,ydim,dim;
  FILE *f1=fopen(nom,"r");
  void *imagen;

  xdim=fgetc(f1)|(fgetc(f1)<8);
  ydim=fgetc(f1)|(fgetc(f1)<8);
  dim=imagesize(0,0,xdim,ydim);
  imagen=malloc(dim);
  rewind(f1);
  fread(imagen,dim,1,f1);
  fclose(f1);
  free(imagen);
  return (imagen);
}
/********************************/

void dos_jugadores(){
  int muertos_usu1=0,muertos_usu2=0,i,j,t=0,c;
  unsigned int n,key,bandera;
  unsigned tam;
  int msg_usu1[4]={0,0,0,0},msg_usu2[4]={0,0,0,0};
  void *sm1,*sm2,*img;

  usu1.x=Px, usu1.y=Py;
  usu2.x=getmaxx()-Px-220, usu2.y=Py;
  n=imagesize(Px+1,Py+1,Px+21,Py+21);
  sm1=malloc(n);  sm2=malloc(n);
  //Dibujo fondo
  setfillstyle(SOLID_FILL,LIGHTGRAY);
  bar(0,0,getmaxx(),getmaxy());
  //Creo la cuadricula
  malla(Px,Py,Px+(22*10),Py+(22*10));
  getimage(usu1.x+1,usu1.y+1,usu1.x+21,usu1.y+21,sm1);
  getimage(usu2.x+1,usu2.y+1,usu2.x+21,usu2.y+21,sm2);
  //USUARIO N�1 UBICA SUS BARCOS
  cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"Pueden moverlos con las flechas, girarlos con G y fijarlos con F");
  settextstyle(7,0,3);
  outtextxy(Px+50,20,"Usuario N�1");
  settextstyle(0,0,0);
  usu1_p1.fijar(mat_usu1);
  usu1_s1.fijar(mat_usu1);
  usu1_l1.fijar(mat_usu1);
  usu1_l2.fijar(mat_usu1);
  usu1_p1.dibujar();
  usu1_s1.dibujar();
  usu1_l1.dibujar();
  usu1_l2.dibujar();
  cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2," ");
  //Dibujo fondo
  setfillstyle(SOLID_FILL,LIGHTGRAY);
  bar(0,0,getmaxx(),getmaxy());
  //Creo la cuadricula
  malla(Px,Py,Px+(22*10),Py+(22*10));
  //USUARIO N�2 UBICA SUS BARCOS
  settextstyle(7,0,3);
  outtextxy(Px+50,20,"Usuario N�2");
  settextstyle(0,0,0);
  usu2_p1.fijar(mat_usu2);
  usu2_s1.fijar(mat_usu2);
  usu2_l1.fijar(mat_usu2);
  usu2_l2.fijar(mat_usu2);
  usu2_p1.dibujar();
  usu2_s1.dibujar();
  usu2_l1.dibujar();
  usu2_l2.dibujar();
  //Copio la matriz de posiciones para la trampa
  for(i=0;i<10;i++)
    for(j=0;j<10;j++){
      tramp[i][j][0]=mat_usu1[i][j];
      tramp[i][j][1]=mat_usu2[i][j];
    }
  //Borro la vista previa
  setfillstyle(SOLID_FILL,7);
  bar(getmaxx()/2-50,Py+22*10+30,getmaxx()/2+51,Py+22*10+31+100+11);
  //Dibujo fondo
  setfillstyle(SOLID_FILL,LIGHTGRAY);
  bar(0,0,getmaxx(),getmaxy());
  //Creo la cuadricula
  malla(Px,Py,Px+(22*10),Py+(22*10));
  malla(getmaxx()-(22*10)-Px,Py,getmaxx()-Px,Py+(22*10));
  cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"Seleccionando quien juega primero...");
  delay(1000);
  bandera=random(2);
  if(bandera==0)
    cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"Comienza jugando el usuario 1");
  else
    cuadro(1,getmaxy()-15,getmaxx()-2,getmaxy()-2,"Comienza jugando el usuario 2");
  setfillstyle(1,7);
  bar(1,getmaxy()-18,getmaxx(),getmaxy());

  settextjustify(LEFT_TEXT,TOP_TEXT);
  settextstyle(7,0,3);
  outtextxy(getmaxx()/4,Py+220+50,"Jugando:");
  do{
    switch(bandera){
      case 0:
	setcolor(4);
	settextjustify(LEFT_TEXT,TOP_TEXT);
	settextstyle(7,0,3);
	outtextxy(getmaxx()/4+130,Py+220+50,"Usuario 1");
	do{
	  dib_mira(usu1.x,usu1.y);
	  key=getch();
	  if(key==27) break;

	  if(key=='t' && t==0) t++;
	  else if(key=='r' && t==1) t++;
	  else if(key=='a' && t==2) t++;
	  else if(key=='m' && t==3) t++;
	  else if(key=='p' && t==4) t++;
	  else if(key=='a' && t==5) t++;
	  else t=0;
	  if(t==6){
	    trampa(getmaxx()-120,getmaxy()-120,tramp,1);
	  }
	  mov_mira1(sm1,key,&usu1.x,&usu1.y);
	  if(key=='d' || key=='D'){
	    putimage(usu1.x+1,usu1.y+1,sm1,COPY_PUT);
	    if(mat_usu2[(usu1.x-Px)/22][(usu1.y-Py)/22]!=2)
	    bandera++;
	    disparar1(usu1.x,usu1.y,mat_usu2,sm1);
	  }
	}while(bandera==0);
	bar(getmaxx()/4+130,80+220+50,getmaxx()/4+250,80+220+50+30);
	bandera=1;
	break;
      case 1:
	setcolor(4);
	settextjustify(LEFT_TEXT,TOP_TEXT);
	settextstyle(7,0,3);
	outtextxy(getmaxx()/4+130,Py+220+50,"Usuario 2");
	do{
	  dib_mira(usu2.x,usu2.y);
	  key=getch();
	  if(key==27) break;

	  if(key=='t' && t==0) t++;
	  else if(key=='r' && t==1) t++;
	  else if(key=='a' && t==2) t++;
	  else if(key=='m' && t==3) t++;
	  else if(key=='p' && t==4) t++;
	  else if(key=='a' && t==5) t++;
	  else t=0;
	  if(t==6){
	    trampa(getmaxx()-120,getmaxy()-120,tramp,0);
	  }
	  mov_mira(sm2,key,&usu2.x,&usu2.y);
	  if(key=='d' || key=='D'){
	    putimage(usu2.x+1,usu2.y+1,sm2,COPY_PUT);
	    if(mat_usu1[10-((getmaxx()-(usu2.x+Px))/22)][(usu2.y-Py)/22]!=2)
	    bandera++;
	    disparar(usu2.x,usu2.y,mat_usu1,sm2);
	  }
	}while(bandera==1);
	bar(getmaxx()/4+130,80+220+50,getmaxx()/4+250,80+220+50+30);
	bandera=0;
	break;
    }
    if(key==27) break;
    usu_destruido(msg_usu1,&muertos_usu1,mat_usu1,usu1_s1,usu1_p1,usu1_l1,usu1_l2);
    usu_destruido(msg_usu2,&muertos_usu2,mat_usu2,usu2_s1,usu2_p1,usu2_l1,usu2_l2);
  }while(muertos_usu1!=4 && muertos_usu2!=4);
  if(muertos_usu1==4){
    setfillstyle(1,7);
    bar(getmaxx()/2-120,getmaxy()/2-20,getmaxx()/2+120,getmaxy()/2+20);
    setcolor(8);
    rectangle(getmaxx()/2-120,getmaxy()/2-20,getmaxx()/2+120,getmaxy()/2+20);
    setcolor(15);
    rectangle(getmaxx()/2-120+1,getmaxy()/2-20+1,getmaxx()/2+121,getmaxy()/2+21);
    settextjustify(CENTER_TEXT,CENTER_TEXT); setcolor(0);
    outtextxy(getmaxx()/2,getmaxy()/2-7,"El Usuario N� 2 destrullo al");
    outtextxy(getmaxx()/2,getmaxy()/2+7,"Usuario N� 1");
    game_over();
  }else if(muertos_usu2==4){
    setfillstyle(1,7);
    bar(getmaxx()/2-120,getmaxy()/2-20,getmaxx()/2+120,getmaxy()/2+20);
    setcolor(8);
    rectangle(getmaxx()/2-120,getmaxy()/2-20,getmaxx()/2+120,getmaxy()/2+20);
    setcolor(15);
    rectangle(getmaxx()/2-120+1,getmaxy()/2-20+1,getmaxx()/2+121,getmaxy()/2+21);
    settextjustify(CENTER_TEXT,CENTER_TEXT); setcolor(0);
    outtextxy(getmaxx()/2,getmaxy()/2-7,"El Usuario N� 1 destrullo al");
    outtextxy(getmaxx()/2,getmaxy()/2+7,"Usuario N� 2");
    game_over();
  }

  free(sm1); free(sm2);
  //Borra posiciones de los barcos
  for(i=0;i<10;i++)
    for(j=0;j<10;j++)
      mat_usu1[i][j]=mat_usu2[i][j]=tramp[i][j][0]=tramp[i][j][1]=0;
}

void reloj(int midx,int midy,int r=100){
  int  i,j;
  char *tiempo;

  setfillstyle(SOLID_FILL,7);
  bar(0,0,getmaxx(),getmaxy());
  settextstyle(TRIPLEX_FONT,HORIZ_DIR,10);
  settextjustify(LEFT_TEXT,TOP_TEXT);
  for(i=4;i>0;i--){
    sound(1000);
    setcolor(BLACK);
    sprintf(tiempo, "%d", i-1);
    outtextxy(midx-30,midy-80,tiempo);
    for(j=90;j<=360;j+=3){
      setcolor(0);
      setfillstyle(SOLID_FILL,7);
      sector(midx,midy,90,j, r,r);
      if(j==180)
	nosound();
      delay(5);
    }
    for(j=0;j<=90;j+=2){
      setcolor(0);
      sector(midx,midy,0,j,r,r);
      setcolor(7);
      line(midx,midy,midx+r,midy);
    }
    delay(200);
  }
}

void creditos(){
  int pol[10],pol1[12];
  int bany[10],banb[10];
  void *img; unsigned n;
  n=imagesize(getmaxx()/2-100+7,45,getmaxx()/2-65,290);
  img=malloc(n);

  //Dibujo bandera colgando
  /**********ROJO**********/
  setfillstyle(SOLID_FILL,4);
  bany[0]=getmaxx()/2-100+7;
  bany[1]=45;
  bany[2]=getmaxx()/2-65;
  bany[3]=185;
  bany[4]=getmaxx()/2-80;
  bany[5]=290;
  bany[6]=getmaxx()/2-100+7;
  bany[7]=185;
  bany[8]=bany[0];
  bany[9]=bany[1];
  fillpoly(5,bany);
  /********AMARILLO*******/
  setfillstyle(SOLID_FILL,YELLOW);
  bany[0]=getmaxx()/2-100+7;
  bany[1]=45;
  bany[2]=getmaxx()/2-65;
  bany[3]=185;
  bany[4]=getmaxx()/2-80;
  bany[5]=290;
  bany[6]=getmaxx()/2-100+7;
  bany[7]=145;
  bany[8]=bany[0];
  bany[9]=bany[1];
  fillpoly(5,bany);
  /**********AZUL*********/
  setfillstyle(SOLID_FILL,1);
  banb[0]=getmaxx()/2-100+7;
  banb[1]=105;
  banb[2]=getmaxx()/2-78;
  banb[3]=195;
  banb[4]=getmaxx()/2-80;
  banb[5]=290;
  banb[6]=getmaxx()/2-100+7;
  banb[7]=145;
  banb[8]=bany[0];
  banb[9]=bany[1];
  fillpoly(5,banb);
  /*************************/
  getimage(getmaxx()/2-100+7,45,getmaxx()/2-65,290,img);
  setcolor(7); setfillstyle(SOLID_FILL,7);
  bar(getmaxx()/2-100+7,45,getmaxx()/2-65,290);

  setcolor(15); setfillstyle(SOLID_FILL,BROWN);
  //Dibujo bara
  bar(getmaxx()/2-5-100,40,getmaxx()/2+5-100,getmaxy()-40);
  rectangle(getmaxx()/2-5-100,40,getmaxx()/2+5-100,getmaxy()-40);
  setcolor(0);
  line(getmaxx()/2-100+5,45,getmaxx()/2-100+7,45);
  line(getmaxx()/2-100+5,getmaxy()-120,getmaxx()/2-100+7,getmaxy()-120);
  line(getmaxx()/2-100+7,45,getmaxx()/2-100+7,getmaxy()-120);

  setcolor(15);
  //Dibujo base
  pol[0]=getmaxx()/2-100-5;
  pol[1]=getmaxy()-40;
  pol[2]=getmaxx()/2-100+5;
  pol[3]=getmaxy()-40;
  pol[4]=getmaxx()/2-100+20;
  pol[5]=getmaxy()-30;
  pol[6]=getmaxx()/2-100-20;
  pol[7]=getmaxy()-30;
  pol[8]=pol[0];
  pol[9]=pol[1];
  fillpoly(5,pol);
  //Dibujo punta
  pol1[0]=getmaxx()/2-100-5;
  pol1[1]=40;
  pol1[2]=getmaxx()/2-100+5;
  pol1[3]=40;
  pol1[4]=getmaxx()/2-100+8;
  pol1[5]=30;
  pol1[6]=getmaxx()/2-100;
  pol1[7]=10;
  pol1[8]=getmaxx()/2-100-8;
  pol1[9]=30;
  pol1[10]=pol1[0];
  pol1[11]=pol1[1];
  setfillstyle(SOLID_FILL,YELLOW);
  fillpoly(6,pol1);

  /***anim bandera colgando***/
  setcolor(0);
  for(n=getmaxy()-185-30;n>=45;n-=10){
    putimage(getmaxx()/2-100+7,n,img,COPY_PUT);
    delay(200);
    setfillstyle(SOLID_FILL,7);
    bar(getmaxx()/2-100+7,n,getmaxx()/2-65,n+290-30);
    line(getmaxx()/2-100+7,45,getmaxx()/2-100+7,getmaxy()-120);
  }
  free(img);
  //Dibujo base
  setcolor(15); setfillstyle(SOLID_FILL,BROWN);
  pol[0]=getmaxx()/2-100-5;
  pol[1]=getmaxy()-40;
  pol[2]=getmaxx()/2-100+5;
  pol[3]=getmaxy()-40;
  pol[4]=getmaxx()/2-100+20;
  pol[5]=getmaxy()-30;
  pol[6]=getmaxx()/2-100-20;
  pol[7]=getmaxy()-30;
  pol[8]=pol[0];
  pol[9]=pol[1];
  fillpoly(5,pol);

  //Dibujo bandera
  setcolor(0); setfillstyle(SOLID_FILL,YELLOW);
  bar(getmaxx()/2-100+7,45,getmaxx()/2,105);
  rectangle(getmaxx()/2-100+7,45,getmaxx()/2,105);
  setfillstyle(SOLID_FILL,BLUE);
  bar(getmaxx()/2-100+7,105,getmaxx()/2,145);
  rectangle(getmaxx()/2-100+7,105,getmaxx()/2,145);
  setfillstyle(SOLID_FILL,RED);
  bar(getmaxx()/2-100+7,145,getmaxx()/2,185);
  rectangle(getmaxx()/2-100+7,145,getmaxx()/2,185);

  setfillstyle(SOLID_FILL,YELLOW);
  bar(getmaxx()/2-7,45+30,getmaxx()/2+80,105+30);
  rectangle(getmaxx()/2-7,45+30,getmaxx()/2+80,105+30);
  setfillstyle(SOLID_FILL,BLUE);
  bar(getmaxx()/2-7,105+30,getmaxx()/2+80,145+30);
  rectangle(getmaxx()/2-7,105+30,getmaxx()/2+80,145+30);
  setfillstyle(SOLID_FILL,RED);
  bar(getmaxx()/2-7,145+30,getmaxx()/2+80,185+30);
  rectangle(getmaxx()/2-7,145+30,getmaxx()/2+80,185+30);

  setcolor(YELLOW);setfillstyle(SOLID_FILL,YELLOW);
  bar(getmaxx()/2,45+8,getmaxx()/2+7,45+29);
  sector(getmaxx()/2,45+8,0,360,7,8);
  sector(getmaxx()/2,45+23,0,360,7,7);
  line(getmaxx()/2-6,45+30,getmaxx()/2-3,45+30);

  setcolor(BLUE);setfillstyle(SOLID_FILL,BLUE);
  bar(getmaxx()/2-6,105+30,getmaxx()/2,105+30-7);
  setcolor(YELLOW);setfillstyle(SOLID_FILL,YELLOW);
  sector(getmaxx()/2,105+30-7,180,270,7,7);

  setcolor(4);setfillstyle(SOLID_FILL,4);
  bar(getmaxx()/2-6,145+30,getmaxx()/2,145+30-7);
  setcolor(BLUE);setfillstyle(SOLID_FILL,BLUE);
  sector(getmaxx()/2,145+30-7,180,270,7,7);

  setcolor(7);setfillstyle(SOLID_FILL,7);
  bar(getmaxx()/2-7,185+30,getmaxx()/2,185+30-7);
  setcolor(4);setfillstyle(SOLID_FILL,4);
  sector(getmaxx()/2,185+30-7,180,270,7,7);

  setcolor(0);
  ellipse(getmaxx()/2,45+8,270,90,7,8);
  ellipse(getmaxx()/2,45+23,90,270,7,7);
  line(getmaxx()/2+7,45+8,getmaxx()/2+7,45+29);
  line(getmaxx()/2-7,45+23,getmaxx()/2-7,45+29);
  arc(getmaxx()/2,105+30-7,180,270,7);
  arc(getmaxx()/2,145+30-7,180,270,7);
  arc(getmaxx()/2,185+30-7,180,270,7);

  /***************TEXT************/
  settextjustify(LEFT_TEXT,TOP_TEXT);
  settextstyle(1,0,4);
  setcolor(0);
  for(int i=0;i<10;i++)
    outtextxy(getmaxx()/2-50+i,getmaxy()/2+i,"HECHO EN COLOMBIA");
  setcolor(15);
  outtextxy(getmaxx()/2-50+i,getmaxy()/2+i,"HECHO EN COLOMBIA");
  setcolor(YELLOW);
  outtextxy(getmaxx()/2-50+i+173,getmaxy()/2+i,"COLO");
  setcolor(BLUE);
  outtextxy(getmaxx()/2-50+i+245,getmaxy()/2+i,"MB");
  setcolor(RED);
  outtextxy(getmaxx()/2-50+i+289,getmaxy()/2+i,"IA");

  settextstyle(1,0,3);
  setcolor(0);
  outtextxy(getmaxx()-(getmaxx()/4)-90+i,getmaxy()/2+40+i,"POR:");
  outtextxy(getmaxx()/2+i,getmaxy()/2+70+i,"John J. Quiroga C.");
  outtextxy(getmaxx()/2-50+i,getmaxy()/2+100+i,"johnquirogac@gmail.com");
  outtextxy(getmaxx()/2-20+i,getmaxy()/2+130+i,"BOGOTA D.C. - 2005");
}

void circ_anim(int color){
  setfillstyle(SOLID_FILL,color);
  for(int i=0;i<getmaxx()/2;i++){
    fillellipse(getmaxx()/2,getmaxy()/2,i,i);
    sound(i+800);
  }
  nosound();
}


int menu(){
  int op=0,col,colr,x=30,y=getmaxy()-30,dir=1;
  Mouse.Mueve(getmaxx()/2,getmaxy()/2-75);
  Mouse.Visualizar();
  randomize();
  do{
    E=Mouse.Evento(Mx,My);
    if(Mouse.Movido()){
      col=random(16); settextjustify(CENTER_TEXT,CENTER_TEXT);
      titulo(40,20,col,col+random(16));
    }
    settextjustify(LEFT_TEXT,TOP_TEXT);
    if(x<=getmaxx()-199 && dir){
      setcolor(7);
      outtextxy(x,y,"JOHN QUIROGA C");
      setcolor(1); x+=1;
      outtextxy(x,y,"JOHN QUIROGA C");
      if(x==getmaxx()-199) dir=0;
    }
    else if(x>=30){
      setcolor(7);
      outtextxy(x,y,"JOHN QUIROGA C");
      setcolor(1); x-=1;
      outtextxy(x,y,"JOHN QUIROGA C");
      if(x==30) dir=1;
    }

    if(b1.MouseinB() || b2.MouseinB() || b3.MouseinB() ||
       b4.MouseinB() || b5.MouseinB())
      Mouse.FijaCursorG(HandCursor);
    else
      Mouse.FijaCursorG(ArrowCursor);
    if(E==LMouseDown || E==LMouseStillDown){
      if(b1.MouseinB()){
	b1.Bdown();
	while(b1.MouseinB() && E==LMouseStillDown) E=Mouse.Evento(Mx,My);
	b1.Bup();
	if(b1.MouseinB() && E==LMouseUp)
	  op=1;
      }
      if(b2.MouseinB()){
	b2.Bdown();
	while(b2.MouseinB() && E==LMouseStillDown) E=Mouse.Evento(Mx,My);
	b2.Bup();
	if(b2.MouseinB() && E==LMouseUp)
	  op=2;
      }
      if(b3.MouseinB()){
	b3.Bdown();
	while(b3.MouseinB() && E==LMouseStillDown) E=Mouse.Evento(Mx,My);
	b3.Bup();
      }
      if(b4.MouseinB()){
	b4.Bdown();
	while(b4.MouseinB() && E==LMouseStillDown) E=Mouse.Evento(Mx,My);
	b4.Bup();
	if(E==LMouseUp && b4.MouseinB()==1)
	  op=4;
      }
      if(b5.MouseinB()){
	b5.Bdown();
	while(b5.MouseinB() && E==LMouseStillDown) E=Mouse.Evento(Mx,My);
	b5.Bup();
	if(E==LMouseUp && b5.MouseinB()==1)
	  op=5;
      }
    }
  }while(op==0);
  bar(x-5,getmaxy()-30,x+200,getmaxy()-30+13);
  return op;
}

/*************Funciones de la presentacion**************/
void rand_colors(){
  int y=0,i=0,veces=0,yi;
  settextstyle(0,0,8);
  settextjustify(CENTER_TEXT,CENTER_TEXT);
  do{
    y=0; yi=getmaxy()/16;
    randomize();
    for(i=0;i<16;i++){
      setfillstyle(SOLID_FILL,random(17));
      bar(0,y,getmaxx(),y+yi);
      y+=yi;
      delay(5);
      sound(i+500);
    }
    setcolor(0);
    outtextxy(getmaxx()/2,200,"NO");
    outtextxy(getmaxx()/2,300,"SIGNAL");
    sound(veces+1500);
    veces++;
  }while(veces<30);
  nosound();
}

void gray_scale(){
  palettetype pal;
  int i;

  settextstyle(1,0,4);
  settextjustify(CENTER_TEXT,CENTER_TEXT);
  getpalette(&pal);
  for(i=0;i<pal.size;i++)
    setrgbpalette(pal.colors[i],i*4,i*4,i*4);
  for(i=pal.size;i>=0;i--){
    setfillstyle(1,i);
    bar(0,0,getmaxx(),getmaxy());
    setcolor(pal.size-i);
    outtextxy(getmaxx()/2,getmaxy()/2,"John Jairo Quiroga Carvajal.");
    delay(200);
  }
}

void text_anim(int x,int y,int font,char *let,int c1,int c2){
  int i,dir=1;
  for(i=1;i<9;i++){
    setcolor(c2);
    outtextxy(x,y,let);
    setcolor(c1);
    settextstyle(font,(dir++==2)?dir=0:dir=dir,i);
    outtextxy(x,y,let);
    sound(x+i*20);
    delay(100);
  }
  nosound();
}

void cuadros(){
  int inc,i;
  setfillstyle(SOLID_FILL,7);
  for(i=0;i<=getmaxx()/2;i++){
    bar(getmaxx()/2,getmaxy()/2,getmaxx()/2+i,getmaxy()/2-i);
    delay(5);
    sound(i+1000);
    bar(getmaxx()/2,getmaxy()/2,getmaxx()/2-i,getmaxy()/2+i);
    delay(5);
    sound(1000-i);
  }
  for(i=0;i<=getmaxx()/2;i++){
    bar(getmaxx()/2,getmaxy()/2,getmaxx()/2-i,getmaxy()/2-i);
    delay(5);
    sound(i+1000);
    bar(getmaxx()/2,getmaxy()/2,getmaxx()/2+i,getmaxy()/2+i);
    delay(5);
    sound(1000-i);
  }
  nosound();
}
/*************************Fin funciones************************/

void main(){
  int i,j,op,x,y;

  ModoVGA();//Inicia el Modo Grafivo y establece una resolucion estandar.
  rand_colors();
  delay(1000);
  setfillstyle(1,15);
  bar(0,0,getmaxx(),getmaxy());
  delay(200);
  gray_scale();
  closegraph();
  ModoVGA();
  setcolor(15);
  setfillstyle(2,BLUE);
  bar(0,0,getmaxx(),getmaxy());
  setfillstyle(SOLID_FILL,0);
  fillellipse(getmaxx()/2,getmaxy()/2,getmaxx()/2-80,getmaxy()/2-80);
  text_anim(150,120,7,"G",15,0);
  ellipse(getmaxx()/2,getmaxy()/2,0,360,getmaxx()/2-80,getmaxy()/2-80);
  text_anim(200,120,7,"U",15,0);
  text_anim(250,120,7,"E",15,0);
  text_anim(300,120,7,"R",15,0);
  text_anim(350,120,7,"R",15,0);
  text_anim(400,120,7,"A",15,0);
  text_anim(250,220,7,"N",15,0);
  text_anim(300,220,7,"A",15,0);
  text_anim(350,220,7,"V",15,0);
  text_anim(400,220,7,"A",15,0);
  text_anim(450,220,7,"L",15,0);
  for(i=0;i<15;i++)
    ellipse(getmaxx()/2-i,getmaxy()/2-i,0,360,getmaxx()/2-80,getmaxy()/2-80);
  setcolor(4);
  for(i=0;i<3;i++)
    ellipse(getmaxx()/2,getmaxy()/2,0,360,getmaxx()/2-80+i,getmaxy()/2-80+i);
  delay(1000);
  //cuadros();
  setcolor(7);
  for (i=0; i<=2100; i++){
    x= rand()%640; y= rand()%480;
    line(0,y, getmaxx(),y);
    x= rand()%640; y= rand()%480;
    line(x,0, x,getmaxy());
    sound(2000-i);
    delay(3);
    nosound();
  }

  for(i=0;i<10;i++)
    for(j=0;j<10;j++)
      mat_usu1[i][j]=mat_usu2[i][j]=0;
  //Dibujo fondo
  setfillstyle(SOLID_FILL,0);
  bar(0,0,getmaxx(),getmaxy());
  InstalaMouseG();
  int x1=40,x2=getmaxx()-40,y1=266,y2=266;
  do{
    //Dibujo fondo
    setfillstyle(1,0);
    bar(0,0,getmaxx(),getmaxy());
    setfillstyle(1,1);
    rect_oval(20,10,getmaxx()-20,getmaxy()/2,7,20);
    rect_oval(20,getmaxy()/2,getmaxx()-20,getmaxy()-10,7,20);
    rect_oval(getmaxx()/2-120,140,getmaxx()/2+120,390,BROWN,20);
    settextjustify(LEFT_TEXT,TOP_TEXT);
    settextstyle(1,0,7);
    titulo(40,20,8,8);
    titulo(50,40,0,15);
    settextstyle(0,0,1);
    b1.Cboton(getmaxx()/2-75,150,getmaxx()/2+75,180,"UN JUGADOR");
    b2.Cboton(getmaxx()/2-75,200,getmaxx()/2+75,230,"MULTI-JUGADOR 1PC");
    b3.Cboton(getmaxx()/2-75,250,getmaxx()/2+75,280,"MULTI-JUGADOR 2PC");
    b4.Cboton(getmaxx()/2-75,300,getmaxx()/2+75,330,"CREDITOS");
    b5.Cboton(getmaxx()/2-75,350,getmaxx()/2+75,380,"SALIR");
    x1=40; x2=getmaxx()-40; y1=266; y2=266;
    setcolor(BROWN);
    for(x1,x2;x1<getmaxx()/2-120,x2>getmaxx()/2+120;x1+=10,x2-=10){
      line(x1,266,getmaxx()/2-120,y1-=6);
      line(x1,266,getmaxx()/2-120,y2+=6);
      line(x2,266,getmaxx()/2+120,y1);
      line(x2,266,getmaxx()/2+120,y2);
      delay(100);
    }

    op=menu();
    switch(op){
      case 1:
	dif=dificultad();
	if(dif==4){ cleardevice(); break; }
	Mouse.DesActiva();
	cleardevice();
	reloj(getmaxx()/2,getmaxy()/2,120);
	un_jugador();
	dif2=0,dir_disp=1;
	Mouse.Activa();
	break;
      case 2:
	Mouse.DesActiva();
	cleardevice();
	reloj(getmaxx()/2,getmaxy()/2,120);
	dos_jugadores();
	Mouse.Activa();
	break;
      case 4:
	Mouse.DesActiva();
	cleardevice();
	circ_anim(7);
	creditos();
	settextjustify(LEFT_TEXT,TOP_TEXT);
	settextstyle(0,0,1);
	outtextxy(getmaxx()/2,getmaxy()-20,"CLICK para salir");
	Mouse.Activa();
	Mouse.Mueve(getmaxx()/2,getmaxy()/2);
	do{
	  E=Mouse.Evento(Mx,My);
	}while(E!=LMouseUp);
	break;
    }
  }while(op!=5);
}