#include <stdio.h>
#include <math.h>
#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define M_PI 3.14

GLfloat angleInDegrees = 0.f;
GLfloat fanDegree = 0.0f;

float move_right = 0 ;
float move_left = 0 ;
float move_up = 0;
float move_down = 0 ;
float move_inside = 0 ;
float move_outside = 0 ;

#define DEF_D 5
#define Cos(th) cos(M_PI/180*(th))
#define Sin(th) sin(M_PI/180*(th))

// kamerayi dondurmek icin aci
float angle=0.0;
// kameranin yonu icin kullaniliyor
float lx=0.0f,lz=-1.0f;
// XZ ekseni
float x=0.0f,z=0.0f;

bool isFly = false; // ucma durumunda olup olmadigini kontrol edecek

// Initialize OpenGL state
void init() {
	printf("Initializing\n");
	glClearColor(0.0, 0.0, 0.0, 1.0);
}
void drawTorus(int numc , int numt,float x_yer,float y_yer, float z_yer)
{  // torus cizmi 
	int i, j, k; 
    double s, t, x, y, z, twopi; 

	twopi = 2 * (double)M_PI;  // 2 pi
 for (i = 0; i < numc; i++)
 { 
	glBegin(GL_QUAD_STRIP); 
		for (j = 0; j <= numt; j++) { 
			for (k = 1; k >= 0; k--) { 
				s = (i + k) % numc + 0.5; 
				t = j % numt; 

			x = (1+.1*cos(s*twopi/numc))*cos(t*twopi/numt );
			y = (1+.1*cos(s*twopi/numc))*sin(t*twopi/numt) ;
			z = .1 * sin(s * twopi / numc); 
		    glVertex3f((x/10)+x_yer,(y/10)+y_yer,(z/10)+z_yer);	
						} 
				} 
	glEnd(); 
	} 
}
void drawBox(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4,float z)
{
	glBegin(GL_POLYGON);
	     glVertex3f(x1, y1, z);       // P1
		 glVertex3f(x2, y2, z);       // P2
		 glVertex3f(x3, y3, z);       // P3
		 glVertex3f(x4, y4, z);       // P4
	glEnd();
}
void drawStick()
{
	//toruslari tutacak cubuklar cizdiriliyor
   drawBox(-0.4,0.02,0.0,0.02,0.0,-0.02,-0.4,-0.02,1.01f);
   drawBox(0.0,0.02,0.4,0.02,0.4,-0.02,0.0,-0.02,1.01f);
   drawBox(0.18,0.31,0.22,0.29,0.02,0.02,-0.02,0.02,1.01f);
   drawBox(-0.18,-0.31,-0.22,-0.29,-0.02,0.02,0.03,0.02,1.01f);
}
void drawToruses()
{
	//4 tane torusu cizdiriyoruz
	drawTorus(8,25,0.4,0,1);
	drawTorus(8,25,-0.4,0,1);
	drawTorus(8,25,0.2,0.3,1);
	drawTorus(8,25,-0.2,-0.3,1);
}
void drawFan_right() //sag taraf icin pervane cizdirme
{
    drawBox(0.3,0.01,0.5,0.01,0.5,-0.01,0.3,-0.01,1);
	drawBox(0.39,0.1,0.41,0.1,0.41,-0.1,0.39,-0.1,1);
}
void drawFan_left() // sol taraf icin pervane cizdirme
{
	drawBox(-0.5,0.01,-0.3,0.01,-0.3,-0.01,-0.5,-0.01,1);
	drawBox(-0.39,0.1,-0.41,0.1,-0.41,-0.1,-0.39,-0.1,1);
}
void drawFan_top() // alt taraf icin pervane cizdirme
{
	drawBox(0.1, 0.31, 0.3, 0.31, 0.3, 0.29, 0.1, 0.29, 1);
	drawBox(0.19, 0.4, 0.21, 0.4, 0.21, 0.2, 0.19, 0.2, 1);
}
void drawFan_bottom() // ust taraf icin pervane cizdirme
{
	drawBox(-0.3, -0.31, -0.1, -0.31, -0.1, -0.29, -0.3, -0.29, 1);
	drawBox(-0.19, -0.2, -0.21, -0.2, -0.21, -0.4, -0.19, -0.4, 1);
}
void drawFans()
{
	drawFan_right();
	drawFan_left();
    drawFan_top();
	drawFan_bottom();	
}
void drawCylinder(int n = 3, int arg = 0, float mult = 1, float v = 1.0) // ortadaki silindirin cizimi
{ 
	int i,j, k;
  glBegin(GL_QUAD_STRIP);
	glTranslatef(0.0f,0.0f,1.0f);
	glColor3f(0.0, 1.0, 0.0);
	for (j = 0; j <= 360; j += DEF_D) {
		glTranslatef(0.0f,0.0f,-1.0f);
		glVertex3f(Cos(j)*0.1, +1*0.1, Sin(j)*0.1);
		glVertex3f(Cos(j)*0.1, -1*0.1, Sin(j)*0.1);
	}
	glEnd();

	/* ust ve alt daireler icin */
	for (i = 1; i >= -1; i -= 2) {
		glTranslatef(0.0f,0.0f,-1.0f);
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0, i*0.01, +0.0);
		for (k = 0; k <= 360; k += DEF_D)
			glColor3f(1.0, 1.0, 1.0);
			glVertex3f(i*Cos(k)*0.01, i*0.01, Sin(k)*0.01);
		}
		glEnd();
}
void drawTriangle(float x1,float y1,float z1 ,float x2, float y2,float z2 , float x3, float y3, float z3) // ortadaki ucgenin cizimi
{
	glBegin(GL_POLYGON);
	     glVertex3f(x1, y1, z1);       // P1
		 glVertex3f(x2, y2, z2);       // P2
		 glVertex3f(x3, y3, z3);       // P3
	glEnd();

}
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
       case 'M':
	   case 'm': /* M/m harfi motoru simgeler eger basilirsa pervanelerin donme durumu islemleri yapilmali
				 tekrar basilirsa motor durmali */
		if (isFly){ // motor calisiyorsa motor kapatiliyor
			isFly = false;
		}
		else{ // motor kapaliysa calistiriliyor
			isFly = true;
		}
		glutPostRedisplay();
		break;

	   case 'D':
	   case 'd': /* quadrocopter i saga kaydirmak icin */
		 move_right = move_right + 0.01f;
		 break;
	   
	   case 'A': /*quadrocopter i sola kaydirmak icin */
	   case 'a':	
		 move_left = move_left + 0.01f;
		 break;
		
	   case 'W': /*quadrocopter i yukari kaydirmak icin */
	   case 'w':
		   move_up = move_up + 0.01f;
		   break;
		
	   case 'S': /*quadrocopter i asagi kaydirmak icin */
	   case 's':
		   move_down = move_down + 0.01f;
		   break;

	   case 'Q':  /*quadrocopterle z ekseninde hareket icin  */
	   case 'q':
		   move_inside = move_inside + 0.01f;
		   break;
		
	   case 'X': /*quadrocopterle z ekseninde hareket icin  */
	   case 'x':
		    move_outside = move_outside + 0.01f;
			break;

	   default: break;	
	}
}
void camera_keys(int key , int x , int y) // kamera tuþlarý ayarlanýyor
{
	float fraction = 0.1f;

	// x ve z eksenli kamera hareketi
	switch (key) { // her seferinde lx ve lz hesaplatilarak , kamerada yerlesitirilerken kullaniliyor
		case GLUT_KEY_LEFT :
			angle -= 0.01f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case GLUT_KEY_RIGHT :
			angle += 0.01f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case GLUT_KEY_UP :
			x += lx * fraction;
			z += lz * fraction;
			break;
		case GLUT_KEY_DOWN :
			x -= lx * fraction;
			z -= lz * fraction;
			break;
	}
}
void turnFans() // cismin pervaneleri dondurmek icin fonksiyon
{
	printf("burdayim");
		glPushMatrix(); // push matris o anki matrisleri save ediyor
			glTranslatef(-0.4f,+0.0f,0); // merkezi -0.4 f e alýyoruz
		    glRotatef(fanDegree,0,0,1);
		    glTranslatef(0.4f,+0.0f,0); //merkezi geri orjine tasiyoruz
			drawFan_left();
		glPopMatrix();
		
		glPushMatrix();
		glTranslatef(0.4f,+0.0f,0); // merkezi 0.4 f e alýyoruz
		    glRotatef(fanDegree,0,0,1);
			glTranslatef(-0.4f,+0.0f,0); //merkezi geri orjine tasiyoruz
			drawFan_right();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(0.2f,+0.3f,0); // merkezi (0.2f,0.3f) e alýyoruz
		    glRotatef(fanDegree,0,0,1);
			glTranslatef(-0.2f,-0.3f,0); // merkezi geri orjine tasiyoruz
			drawFan_top();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-0.2f,-0.3f,0); // merkezi (0.2f,0.3f) e alýyoruz
			glRotatef(fanDegree,0,0,1);
			glTranslatef(+0.2f,+0.3f,0); // merkezi geri orjine tasiyoruz
			drawFan_bottom();
		glPopMatrix();

	glFlush();
}
void timer_function(int n) // sonsuz dongu ile süreki timerin cagirilmasini saglayacak
{//timer fonksiyonunda parametre olarak value istedigi icin int value yazildi
    if(isFly)// isFly true ise yani ucuyorsa
	{
	printf("timer_function ici");
	fanDegree = fanDegree + 0.2f; // pervanelerde donme acisi oldugu icin her seferinde arttiriliyor
	//glutPostRedisplay();
	} 
	glutTimerFunc(10000,timer_function, 100); // esas timer fonksiyonu , pervane donus hizinda etkili
}
/*void drawCoordinates() // ekranda x,y,z eksenini gostermek icin
{
  drawBox(-0.01f , 0 , -0.01f , 50.0f , +0.01f , 50.0f , +0.01f , 0, 0 ); // y ekseni
  drawBox( 0.0f  , -0.01f ,  0 , +0.01f , 50.0f, +0.01f , 50.0f , +0.01f , 0 ); // x ekseni
	// to do  = z eksenii !!!
} */
void renderScene()  // cizdirme islemleri
{
	printf("Window needs rendering\n");

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glMatrixMode(GL_MODELVIEW);      // To operate on Model-View matrix


	for(int i = 0 ; i<20;i++) // XZ duzlemine 20 tane cizdiriliyor !
	{
	    glPushMatrix();


	    // Reset transformations
		glLoadIdentity();
		// kamera ayarlaniyor
		gluLookAt(	x, 0.75f, z,
					x+lx, 2.0f, z+lz+0.1f,
					0.0f, 1.0f, 1.0f);      
	 
	 // saga-sola , yukari-asagi , iceri-disari hareket icin  ,
     if(isFly)// isFly true ise yani ucuyorsa
	{
	 glTranslatef (move_right-move_left+i, move_up-move_down, move_inside-move_outside+i);
	}
	 else
	 {
		 glTranslatef(i,0,i);// i parametresi urettigimiz quadrocopteri kopyalarken kullaniliyor !
	 }

	 drawCylinder(3, 0, 0.5, 4); // silindiri cizdiriyoruz

  	glColor3f(0.0,0.0,1.0); //cizdirilecek cubuklari rengi
	//toruslari tutacak cubuklar cizdiriliyor
	drawStick();
	
	glColor3f(0.0,1.0,0.0); // cizdirilecek toruslarin rengi
	//4 tane torusu cizdiriyoruz
	drawToruses();

	glColor3f(0.4f,0.6f,1.0f); 
	drawTriangle(-0.1f,+0.2f,0.9f,0.1f,+0.2f,0.9f,0,-0.1f,0.9f); // ucgen cizdiren fonksiyon

	glColor3f(1.0,0.0,0.0); // cizdirilecek pervanelerin rengi
    // 4 torus icin 4 pervane cizdiriliyor
	

	if(isFly)// isFly true ise yani ucuyorsa
	{
		turnFans(); // donme durumunda fanlarin hali 
		timer_function(0);
	}
	else // ucmuyorsa
	{
		drawFans(); // donmeme durumunda fanlarin hali
	}

	glPopMatrix();

		}
	glFlush();  // Render now
	glutSwapBuffers();

	
}
void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(0.0, (GLfloat) w/(GLfloat) h,0.0, 20.0);
	glMatrixMode(GL_MODELVIEW); // modelview matris
	glLoadIdentity();
	gluLookAt (0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}
void idle() { //loop yapilmasini sagliyor 
   glutPostRedisplay();   // Post a re-paint request to activate display()
}
int main(int argc, char **argv) {
	// init GLUT and create window
	glutInit(&argc, argv); // don't remove this line
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("PROJE2B : Soru 2");

	init(); // Initialize OpenGL state in this

	// OpenGL init
	glEnable(GL_DEPTH_TEST);
	
	glutDisplayFunc(renderScene);
	glutKeyboardFunc(keyboard); // keyboard fonksiyonu tanimlaniyor
	glutSpecialFunc(camera_keys); // yon tuslari kamera icin ayarlaniyor
	glutReshapeFunc(reshape);       // ekranin seklini ayarlayan fonksyion
	//glutTimerFunc(100,timer_function,0);
	glutIdleFunc(renderScene); // her seferinden tekrar cizdirme icin initialize ediliyor
	glutMainLoop();
	

	return 0;
}


