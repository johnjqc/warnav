#ifndef __RATON_H
#define __RATON_H

#include <graphics.h>
#include <conio.h>
#include <dos.h> //para getvect,REGS,int86,
				 //MK_FP,int86x,SREGS,intr,REGPACK
#include <stdlib.h> //para NULL

typedef unsigned char byte;
typedef unsigned int word;
enum boolean{false,true};
boolean Boton1,Boton2;
byte Activo;

unsigned E;
int Mx,My;

 //define estilos para el cursor
 struct Pointers{ int X,Y; };
 struct CursorRaton{
  Pointers Puntos;
  unsigned MascaraPantalla[16];
  unsigned MascaraCursor[16];
 };
 extern const CursorRaton ArrowCursor;
 extern const CursorRaton HandCursor;
 extern const CursorRaton LeftRightCursor;
 extern const CursorRaton UpDownCursor;
 extern const CursorRaton PencilCursor;
 extern const CursorRaton WaitCursor;
 extern const CursorRaton CircleCursor;
 extern const CursorRaton HammerCursor;
 extern const CursorRaton DisketteCursor;
 extern const CursorRaton CrossCursor;
 extern const CursorRaton JotaCursor;
 //eventos del raton
 const unsigned Idle            = 0x0000;
 const unsigned MouseDown       = 0xff01;
 const unsigned LMouseDown      = 0xff01;
 const unsigned RMouseDown      = 0xff02;
 const unsigned MouseStillDown  = 0xff04; //Original 0xff04
 const unsigned LMouseStillDown = 0xff04; //Original 0xff04
 const unsigned RMouseStillDown = 0xff08; //Original 0xff08
 const unsigned MouseUp         = 0xff10;
 const unsigned LMouseUp        = 0xff10;
 const unsigned RMouseUp        = 0xff20;
 const unsigned MouseEnter      = 0xff40;
 const unsigned MouseLeave      = 0xff80;
 const unsigned MouseWithin     = 0xffc0;
 const int ResetMouse=0;
 const int GetMouseStatus=3;
 //mascaras para el raton
 const unsigned LeftButton      = 0x0001;
 const unsigned RightButton     = 0x0002;
 //modos de video
 enum TipoModoVideo{TextoScrn,AltaResGr,Vga,Grafico};
 //clase raton
 class Raton{
  protected:
   int OldX,OldY;   //para mantener la posicion
   char Bien;       //verdadero si el raton esta inicializado
   char RatonOff;   //verdadero si el raton esta inactivo
   char AltaRes;    //verdaero si esta esta en la modalidad grafica 640*480
   char ModoTexto;  //verdadero si esta en modo texto
  public:
   int X,Y,Dx,Dy;// para el movimiento del raton
   Raton(void);
   ~Raton(void);
   void Instalacion(TipoModoVideo ModoVideo);
   int ExisteRaton();
   int InstalacionOk();
   void Ocultar();
   void Visualizar();
   void ControladorASM(void);
   void LimitaAreaRaton(int x1, int y1, int x2, int y2);
	 void ColocaRatonXY(int x, int y);
   unsigned Estado(int &Mx,int &My);
   boolean Inbox(int left,int top,int right,int bottom,int x,int y);
   unsigned EstadoBoton(void);
   int PresionoCnt(unsigned MascaraBoton);
   int ReleaseCnt(unsigned MascaraBoton);
   unsigned Evento(int &Mx,int &My);
   void InstalaControlador(word Segmento,word Desplazamiento,word Valor);
   void FijaAreaExclusion(int x1,int y1,int x2,int y2);
   void EsperaEvento(unsigned E,int &Mx,int &My);
   void DesActiva();
   void Activa();
   void FijaCursorG(const CursorRaton &NuevoCursor);
   int Operando();
   virtual int Reset();
   virtual void Mueve(int Mx,int My);
   virtual int Movido();
   virtual void GetCoords(int &x,int &y);
   virtual void MouseIntr(int &m1,int &m2,int &m3,int &m4);
   unsigned EsperaOcurraEvento(int &Mx,int &My);
 };

const int Iret=0xCf;
Raton  Mouse;

void InstalaMouseG(void){
	Mouse.Instalacion(Grafico);
	if(!Mouse.Operando()){
		closegraph();
		clrscr();
		cprintf("Raton no detectado");
		getch();
		exit(1);
	}
}


//inicializa el raton a un estado conocido.
//El raton se encuentra inactivo todavia
Raton::Raton(void){
 Bien=false;
 RatonOff=true;
}
Raton::~Raton(void){ ; }
//Devuelve verdadero si el controlador del raton esta instalado.
//La funcion se asegura de la ubicacion del vector de interrupcion
int Raton::ExisteRaton(void){
	void far *direccion;
	//Busca direccion NULL o instruccion IRET
	direccion=getvect(0x33);
	return (direccion!=NULL) && (*(byte far*)direccion!=Iret);
}

//Inicializa el objeto raton verificando que exista el controlador del raton
void Raton::Instalacion(TipoModoVideo ModoVideo){
	REGS r;
	Bien=ExisteRaton();
	if(Bien){ //Raton presente
	 r.x.ax=0;
	 int86(0x33,&r,&r);
	 if(r.x.ax==0) Bien=false;
	}
	if(!Bien){   //Inicializacion del raton fallo
	 DesActiva();//La condicion del raton en OFF y regresa
	 return;
	}
	Activa();    //Fija la condicion del raton en ON
	if(ModoVideo==TextoScrn)  ModoTexto=true;
	else ModoTexto=false;
	if(ModoVideo==AltaResGr) AltaRes=true;
	else AltaRes=false;
	//Fija el modo VGA
	if(ModoVideo==Vga) *(char far*) MK_FP(0xA000,0x0000)=0x12;
	OldX=0; OldY=0;  //Inicializa varias variables de instancia
	X=0; Y=0;
	Dx=0; Dy=0;
	Mueve(0,0);//Lleva al raton hasta el extremo superior izquierdo de la pantalla
}

//Devuelve verdadero solo si la inicializacion del raton es exitosa
int Raton::InstalacionOk(void){
 return Bien;
}

//Oculta el cursor del raton (elimina el cursor de la pantalla)
void Raton::Ocultar(void){
	REGS r;
	if(!Operando()) return;
	r.x.ax=2;
	int86(0x33,&r,&r);
}
//Visualiza el cursor del raton
void Raton::Visualizar(void){
 REGS r;
 if(!Operando()) return;
 r.x.ax=0x01;
 int86(0x33,&r,&r);
}
//Retorna la ubicacion del raton en Mx y My y el
//estado actual de los botones
unsigned Raton::Estado(int &Mx,int &My){
	REGS r;
	if(!Operando()) {
	 Mx=0; My=0;
	 return false;
	}
	r.x.ax=0x03;
	int86(0x33,&r,&r);
	Mx=r.x.cx; My=r.x.dx;
	if(ModoTexto){
	 Mx>>=3;//ajusta para coordenadas de texto
	 My>>=3;
	}
	if(AltaRes) Mx>>=1; //Ajusta para coordenadas de 640*480
	return r.x.bx;
}
//Establece la posicion del apuntador del raton
 void Raton::ColocaRatonXY(int x, int y){
	REGS r;
	if(!Operando()) return;
	r.x.ax=0x04;
	r.x.cx=x;  //posicion horizontal
	r.x.dx=y;  //posicion vertical
	int86(0x33,&r,&r);
}
//Retorna la condicion de los botones del raton
unsigned Raton::EstadoBoton(void){
 int Mx,My;
 if(!Operando()) return false;
 else return Estado(Mx,My);
}
//Retorna el numero de veces que el boton ha sido pulsado
//desde la ultima llamada
int Raton::PresionoCnt(unsigned MascaraBoton){
	REGS r;
	if(!Operando()) return false;
	r.x.ax=0x05;
	r.x.bx=MascaraBoton>>1;
	int86(0x33,&r,&r);
	return r.x.bx;
}
//Devuelve el numero de veces que el boton ha sido liberado
//desde la ultima llamada
int Raton::ReleaseCnt(unsigned MascaraBoton){
 REGS r;
 r.x.ax=0x06;
 r.x.bx=MascaraBoton>>1;
 int86(0x33,&r,&r);
 return r.x.bx;
}
//Fija limites horizontales y verticales para el apuntador del raton
void Raton::LimitaAreaRaton(int x1, int y1, int x2, int y2){
	REGS r;
	if(!Operando()) return;

	r.x.ax=0x07; //peticion para fijar limite horizontal
	r.x.cx=x1; //limite inferior
	r.x.dx=x2; //limite superior
	int86(0x33,&r,&r);

	r.x.ax=0x08; //peticion para fijar limite vertical
	r.x.cx=y1; //limite inferior
	r.x.dx=y2; //limite superior
	int86(0x33,&r,&r);

}
//Controlador de eventos del rat¢n.
//Se ocupar  de ver el estado de los botones del raton
void Raton::ControladorASM(void){
	asm{
	 mov ax,seg(Boton1)//Ponemos el segmento de datos el valor adecuado
	 mov ds,ax
	 cmp Activo,1
	 je et3
	 mov Activo,1  //Ya estamos en el controlador
	 mov Boton1,0
	 mov Boton2,0
	 push bx       //Guardamos el estado del rat¢n
	 and bx,1      //¨Se puls¢ bot¢n 1?
	 jz et1
	 mov Boton1,1
	}
	et1: asm{
	pop bx       //Recupero el estado del rat¢n
	push bx      //y lo vuelvo a guardar
	and bx,2     //¨Se pulso boton 2?
	jz et2
	mov Boton2,1 //Bot¢n 2 a true
	}
	et2: asm{
	pop bx       //Recupero el estado del rat¢n
	mov Activo,0 //Abandonamos el controlador
	}
	et3: asm ret   //Finalizamos con ret no con Iret
}
//Instala manejador de interrupciones para eventos del raton
void Raton::InstalaControlador(word Segmento,word Desplazamiento,word Valor){
	 union REGS regs;
	 struct SREGS segregs;
	 regs.x.ax=0x0C;//Peticion para instalar controlador
	 regs.x.cx=Valor;//M scara de eventos
	 regs.x.dx=Desplazamiento;//Offset del controlador
	 segregs.es=Segmento;//Segmento del controlador
	 int86x(0x33,&regs,&regs,&segregs);
}
//Fija area de exclusion del apuntador del raton.
//Define una area en la pantalla en la que el apuntador no es mostrado.
void Raton::FijaAreaExclusion(int x1,int y1,int x2,int y2){
	REGPACK r;
	r.r_ax=0x10;
	r.r_cx=x1;//coordenada x de la esquina superior izquierda
	r.r_dx=y1;//coordenada y de la esquina superior izquierda
	r.r_si=x2;//coordenada x de la esquina inferior derecha
	r.r_di=y2;//coordenada y de la esquina superior derecha
	intr(0x33,&r);
}
//Obtiene el ultimo evento del raton.
//El boton izquierdo del raton tiene prioridad sobre el raton derecho
unsigned Raton::Evento(int &Mx,int &My){
 unsigned E;
 if(!Operando()){
	Mx=0;
	My=0;
	return Idle;
 }
 //obtiene estado actual de los botones
 E=Estado(Mx,My);
 if(E==0){
 //Ningun boton presionado, quiza hubo un boton pulsado que no fue
 //detectado. Si no, verifica si hubo una liberacion de boton fallida.
 //El boton izquierdo tiene prioridad.
	if(PresionoCnt(LeftButton)>0)         E=LMouseDown;
	else if(PresionoCnt(RightButton)>0)   E=RMouseDown;
	else if(ReleaseCnt(LeftButton)>0)     E=LMouseUp;
	else if(ReleaseCnt(RightButton)>0)    E=RMouseUp;
 }
 else{ //un boton esta presionado
	if(E & LeftButton){  //esta presionado el izquierdo
	 if(PresionoCnt(LeftButton)>0)//no esta pulsado
	E=LMouseDown;    //puede haber estado presionado ya
	 else
	E=LMouseStillDown;
	}
	 else if(PresionoCnt(RightButton)>0)
	 E=RMouseDown;  //puede haber estado presionado ya
	 else
	 E=RMouseStillDown;
 }
 return E;  //devuelve el evento
}

//Espera a que ocurra un evento de raton
unsigned Raton::EsperaOcurraEvento(int &Mx,int &My){
 unsigned E;
 if(!Operando()){
	Mx=0;
	My=0;
	return Idle;
 }
 do{
	 E=Evento(Mx,My);
 }while(E==Idle);
 return E;
}
//Espera que ocurra E. Retorna las coordenadas del raton.
void Raton::EsperaEvento(unsigned E,int &Mx,int &My){
	unsigned Etry;
	if(!Operando()){
	 Mx=0; My=0;
	 return;
	}
	do{
	 Etry=Evento(Mx,My);
	}while(Etry!=E);
}

//Verifica si el raton se ha movido desde la ultima vez que se llamo
//este metodo
int Raton::Movido(void){
 if(!Operando()) return false;
 OldX=X; OldY=Y;
 Estado(X,Y);
 Dx=X-OldX; Dy=Y-OldY;
 return (Dx!=0) || (Dy!=0);
}
//Mueve el cursor del raton
void Raton::Mueve(int Mx,int My){
	REGS r;
	if(!Operando()) return;
	r.x.ax=4;
	r.x.cx=Mx;
	r.x.dx=My;
	if(ModoTexto){//ajusta coordenadas de texto
	 r.x.cx<<=3;
	 r.x.dx<<=3;
	}
	if(AltaRes) r.x.cx<<=1; //ajusta coordenadas 640*480
	int86(0x33,&r,&r);
}
//Activa el raton
void Raton::Activa(void){
 if(Bien && RatonOff){
	RatonOff=false;
	Visualizar();
 }
}
//Desactiva el raton
void Raton::DesActiva(void){
 if(Bien && !RatonOff){
	Ocultar();
	RatonOff=true;
 }
}
//Devuelve un indicador booleano verdadero solo si el objeto
//raton ha sido activado.
int Raton::Operando(void){
	return !RatonOff;
}
//Fija el cursor grafico del raton al tipo especificado.
void Raton::FijaCursorG(const CursorRaton &NuevoCursor){
 REGS r;
 SREGS sr;
 r.x.ax=9;
 r.x.bx=NuevoCursor.Puntos.X;
 r.x.cx=NuevoCursor.Puntos.Y;
 r.x.dx=FP_OFF(NuevoCursor.MascaraPantalla);
 sr.es=FP_SEG(NuevoCursor.MascaraPantalla);
 int86x(0x33,&r,&r,&sr);
}
//Esta funcion proporciona la comunicacion entre el manejador del raton
//y el programa de aplicacion. Los parametros se intercambian con el
//manejador por medio de los registros ax,bx,cx y dx.
void Raton::MouseIntr(int &m1,int &m2,int &m3,int &m4){
 union REGS inregs,outregs;
 inregs.x.ax=m1; inregs.x.bx=m2;
 inregs.x.cx=m3; inregs.x.dx=m4;
 int86(0x33,&inregs,&outregs);
 m1=outregs.x.ax; m2=outregs.x.bx;
 m3=outregs.x.cx; m4=outregs.x.dx;
}
//Reinicia el cursor del raton a: centro de la pantalla, cursor oculto, usando
//el cursor de flecha y con los rangos minimo y maximo puestos a las dimensiones
//maximas de la pantalla virtual. Si existe un manejador de raton, esta funcion
//retorna 1. En caso contrario regresa 0.
int Raton::Reset(void){
 int m1,m2,m3,m4;
 m1=ResetMouse;
 MouseIntr(m1,m2,m3,m4);
 return m1;
}
//Obtiene la posicion actual del cursor del raton
void Raton::GetCoords(int &x,int &y){
 int m1,m2;
 m1=GetMouseStatus;
 MouseIntr(m1,m2,x,y);
 if(getmaxx()==639) x/=2;
}
//Prueba si el cursor del raton se encuentra en el cuadro especificado. Regresa True
//si el cursor esta en el cuadro. En caso contrario la funcion regresa False.
boolean Raton::Inbox(int left,int top,int right,int bottom,int x,int y){
 return ((x>=left && x<=right && y >=top && y<=bottom) ? true : false);
}

const CursorRaton ArrowCursor={
 { 0, 0 }, //Fija en la punta de  la flecha
 { 0x3FFF,0x1FFF,0x0FFF,0x07FF, //Mascara de pantalla
	 0x03FF,0x01FF,0x00FF,0x007F,
	 0x003F,0x00FF,0x01FF,0x10FF,
	 0x30FF,0xF87F,0xF87F,0xFC3F},//Mascara del cursor
 { 0x0000,0x4000,0x6000,0x7000,
   0x7800,0x7C00,0x7E00,0x7F00,
   0x7F80,0x7E00,0x7C00,0x4600,
   0x0600,0x0300,0x0300,0x0180}
};

const CursorRaton HandCursor={
 { 4, 0 }, //Fija en la punta del dedo indice
 {0xF3FF,0xE1FF,0xE1FF,0xE1FF,  //Mascara de pantalla
  0xE04F,0xE001,0xE000,0x8000,
  0x0000,0x0000,0x0000,0x0000,
  0x0000,0x0001,0x8003,0xC007},
 {0x0000,0x0C00,0x0C00,0x0C00,  //Mascara de cursor
  0x0C00,0x0DB0,0x0DB6,0x0DB6,
  0x6DB6,0x6FF6,0x6FFE,0x7FFE,
  0x7FFE,0x7FFC,0x3FF8,0x0000}
 };

const CursorRaton LeftRightCursor={
 { 8, 8},     //Fija en el centro de la flecha
 { 0xffff,0xffff,0xfbdf,0xf3cf, //Mascara de pantalla
   0xe3c7,0xc003,0xe3c7,0xf3cf,
   0xfbdf,0xffff,0xffff,0xffff},
 { 0x0000,0x0000,0x0420,0x0c30, //Mascara del cursor
   0x1428,0x27e4,0x4002,0x8001,
   0x4002,0x27e4,0x1428,0x0c30,
   0x0420,0x0000,0x0000,0x0000}
 };

const CursorRaton  UpDownCursor={
 { 8,8},         //Fija en el centro de la flecha
 { 0xfeff,0xfcff,0xf83f,0xf01f, //Mascara de pantalla
   0xe00f,0xc007,0xf83f,0xf83f,
   0xf83f,0xf83f,0xc007,0xe00f,
   0xf01f,0xf83f,0xfc7f,0xfeff},
 { 0x0100,0x0280,0x0440,0x0820,//Mascara del cursor
   0x1010,0x3c78,0x0440,0x0440,
   0x0440,0x0440,0x3c78,0x1010,
   0x0820,0x0440,0x0280,0x0100}
 };

 const CursorRaton WaitCursor={
  {7, 7 },
  {0xC003, 0xC003, 0xC003, 0xC003,//Mascara de pantalla
   0xE007, 0xF00F, 0xF81F, 0xFC3F,
   0xFC3F, 0xF81F, 0xF00F, 0xE007,
   0xC003, 0xC003, 0xC003, 0xC003},
  {0x0000, 0x1FF8, 0x0000, 0x1FF8,//Mascara del cursor
   0x0D50, 0x06A0, 0x0340, 0x0180,
   0x0180, 0x03C0, 0x07E0, 0x0D50,
   0x1AA8, 0x0000, 0x1FF8, 0x0000}
};
 const CursorRaton PencilCursor={
  {0, 15 },
  {0xFFF1, 0xFFE0, 0xFFC0, 0xFF80, //Mascara de pantalla
   0xFF01, 0xFE03, 0xFC07, 0xF80F,
   0xF01F, 0xE03F, 0xE07F, 0xC0FF,
   0xC1FF, 0x87FF, 0x1FFF, 0x3FFF},
  {0x0000, 0x000E, 0x001C, 0x003A, //Mascara del cursor
   0x0074, 0x00E8, 0x01D0, 0x03A0,
   0x0740, 0x0E80, 0x0D00, 0x1A00,
   0x1800, 0x2000, 0x4000, 0x0000}
 };

 const CursorRaton DisketteCursor={
 {0, 0 },
 {0x8001,0x0000,0x0000,0x0000,
  0x0000,0x0000,0x0000,0x0000,
  0x0000,0x0FF0,0x0810,0x0FF0,
  0x0810,0x0FF0,0x0FF0,0x8001},
 {0x7FFE,0x9411,0x9411,0x9411,
  0x9411,0x8FE1,0x8001,0x8001,
  0x8FF1,0x9009,0x97E9,0x9009,
  0x97E9,0x9009,0x9009,0x7FFE}
 };

 const CursorRaton CrossCursor={
 {8,8},
 {0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF,
  0xFFFF,0xFFFF,0xFFFF,0xFFFF},
 {0x4002,0xE007,0x700E,0x381C,
  0x1C38,0x0E70,0x07E0,0x03C0,
  0x03C0,0x07E0,0x0E70,0x1C38,
  0x381C,0x700E,0xE007,0x4002}
 };

 const CursorRaton CircleCursor={{8,8},
 0xF01F,0xE00F,0xC007,0x8003,
 0x0441,0x0C61,0x0381,0x0381,
 0x0381,0x0C61,0x0441,0x8003,
 0xC007,0xE00F,0xF01F,0xFFFF,
 0x0000,0x07C0,0x0920,0x1110,
 0x2108,0x4004,0x4004,0x783C,
 0x4004,0x4004,0x2108,0x1110,
 0x0920,0x07C0,0x0000,0x0000};

 const CursorRaton HammerCursor={{8,4},
 0x8003,0x0001,0x0001,0x1831,
 0x1011,0x0001,0x0001,0x8003,
 0xF83F,0xF83F,0xF83F,0xF83F,
 0xF83F,0xF83F,0xF83F,0xF83F,
 0x0000,0x3FF8,0x4284,0x4104,
 0x4284,0x4444,0x3FF8,0x0380,
 0x0380,0x0380,0x0380,0x0380,
 0x0380,0x0380,0x0380,0x0000};

 const CursorRaton JotaCursor={{0,0},
 0x1FFF,0x3FFF,0x7801,0xF000,
 0xF000,0xF000,0xF70E,0xFF0F,
 0xFF0F,0xFF0F,0xFF0F,0xF707,
 0xF00F,0xF00F,0xF00F,0xF81F,

 0xE000,0xC000,0x87FE,0x0801,
 0x0801,0x0F9F,0x0891,0x0090,
 0x0090,0x0090,0x0090,0x0890,
 0x0F90,0x0810,0x0810,0x07E0};



#endif
