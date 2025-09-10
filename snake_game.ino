#include <TFT_eSPI.h>
#include <SPI.h>

// 디스플레이 클래스
TFT_eSPI tft = TFT_eSPI();

/***************************

****************************/

// **매크로** //
// 조이스틱 핀
#define JOY_X 14
#define JOY_Y 13
#define JOY_SW 12

// 방향
#define UP    0
#define RIGHT 1
#define DOWN  2
#define LEFT  3

// 뱀 최대 길이
#define MAX_SNAKE_LENGTH 100


// **전역 변수** //
// 뱀 초기 길이
int snakeLength = 3;

// 방향 정의 (0=위, 1=오른쪽, 2=아래, 3=왼쪽)
int dir = RIGHT; 
int prevDir = RIGHT;

// 화면 칸
const int GRID_SIZE = 10;
const int GRID_WIDTH = 320 / GRID_SIZE;
const int GRID_HEIGHT = 240 / GRID_SIZE;


// **구조체** //
// 좌표 구조체
struct Point
{
  int x;
  int y;
};
// 뱀의 몸통을 저장할 배열
Point snake[MAX_SNAKE_LENGTH];
// 사과 구조체
Point fruit; 
Point prevFruit = {500,500};



// 뱀의 초기 위치 설정
void initSnake()
{
  // 뱀의 초기위치는 화면의 중앙
  int startX = GRID_WIDTH / 2;
  int startY = GRID_HEIGHT / 2;

  // 뱀의 몸통 초기 좌표 설정
  for (int i = 0; i < snakeLength; i++)
  {
    snake[i].x = startX - i; // 왼쪽으로 하나 씩 생성
    snake[i].y = startY;
  }
}

// 뱀 그리기
void drawSnake()
{
  for (int i = 0; i < snakeLength; i++)
  {
    // 뱀의 머리 그리기
    if (i == 0)
    {
      //tft.fillRect(사각형의 왼쪽 위 모서리 x좌표, 왼쪽 위 모서리 y좌표, 가로길이, 세로길이, 컬러 );
      tft.fillRect(snake[i].x * GRID_SIZE, snake[i].y * GRID_SIZE, GRID_SIZE, GRID_SIZE, TFT_BLUE);
    }
    else //뱀의 몸통 그리기
    {
      tft.fillRect(snake[i].x * GRID_SIZE, snake[i].y * GRID_SIZE, GRID_SIZE, GRID_SIZE, TFT_GREEN);
    }
  }
}


// 뱀의 이동
void moveSnake() 
{
  // 꼬리 좌표 저장
  int tailX = snake[snakeLength - 1].x;
  int tailY = snake[snakeLength - 1].y;
  
  // 몸통은 뒤에서부터 앞으로 이동
  for (int i = snakeLength - 1; i > 0; i--) 
  {
    snake[i] = snake[i - 1];
  }

  // 머리 이동
  if      (dir == UP)    snake[0].y--;  // 위
  else if (dir == RIGHT) snake[0].x++;  // 오른쪽
  else if (dir == DOWN)  snake[0].y++;  // 아래
  else if (dir == LEFT)  snake[0].x--;  // 왼쪽

  //벽 충돌시 뱀 정지
  if (snake[0].x < 0 || snake[0].x >= GRID_WIDTH || snake[0].y < 0 || snake[0].y >= GRID_HEIGHT)
  {
    gameOver();
  }
 
  //꼬리 지우기
  tft.fillRect(tailX * GRID_SIZE, tailY * GRID_SIZE, GRID_SIZE, GRID_SIZE, ILI9341_BLACK);

}


// 사과 생성
void spawnFruit() 
{
  //사과의 좌표는 화면 내 랜덤위치
  fruit.x = random(0, GRID_WIDTH);
  fruit.y = random(0, GRID_HEIGHT);
  
  //사과 위치 저장
  prevFruit.x = fruit.x;
  prevFruit.x = fruit.y;
}


// 뱀이 사과를 먹으면 랜덤위치로 사과 생성 + 뱀 길이 1 증가
int checkFruit() 
{
  // 뱀이 사과와 접촉했는지 반복 확인
  for(int i = 0; i < snakeLength; i++)
  {
    
    if (snake[i].x == fruit.x && snake[i].y == fruit.y) 
    {
      snake[snakeLength]=snake[snakeLength - 1];  // 추가 될 꼬리 좌표를 몸통 끝으로 초기화
      
      snakeLength++;  
      
      spawnFruit();  
      return 1;
    }
  }
  return 0;
}


// 사과 그리기
void drawFruit()
{
  // 사과 그린 후 이전에 그려진 사과는 지우기
  tft.fillRect(fruit.x * GRID_SIZE, fruit.y * GRID_SIZE, GRID_SIZE, GRID_SIZE, TFT_RED);
  // tft.fillRect(사각형의 왼쪽 위 모서리 x좌표, 왼쪽 위 모서리 y좌표, 가로길이, 세로길이, 컬러 );
  tft.fillRect(prevFruit.x * GRID_SIZE, prevFruit.y * GRID_SIZE, GRID_SIZE, GRID_SIZE, TFT_BLACK);
}


// 조이스틱 센서 읽기
void readJoystick() 
{
  //조이스틱 움직임 기준치
  static const int THRESHOLD = 200;

  // 조이스틱 영점 보정
  int x = analogRead(JOY_X) - 1800;
  int y = analogRead(JOY_Y) - 1800;

if (abs(x) > THRESHOLD || abs(y) > THRESHOLD) // 조이스틱이 일정값 이상 움직이지 않으면 동작 x
{
    // 방향 변경
    if (abs(x) > abs(y)) 
    {
        dir = (x > 0) ? RIGHT : LEFT;
        
    } else 
    {
        dir = (y > 0) ? DOWN : UP;
  
    }
} else 
{
    // 이전 방향 유지
    dir = prevDir;
}
}


// 몸통 충돌 체크
int checkCollision() 
{
  for (int i = 1; i < snakeLength; i++) 
  {
    if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) 
    {
      return 1;
    }
  }
  return 0;
}


// 게임 오버
void gameOver() 
{
  tft.fillScreen(ILI9341_RED);
  tft.setCursor(80, 120);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);
  tft.println("GAME OVER");
  while(1);
}


// 메인 함수
void setup() 
{
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);

  initSnake();     
  spawnFruit();     
}

void loop() 
{
  readJoystick();    
  moveSnake();       
  checkFruit();

  if (checkCollision() == 1) 
  {
    gameOver();
  }

  drawSnake();       
  drawFruit();        
  prevDir = dir; // 방향 업데이트
  delay(150);        
}