#define _CRT_SECURE_NO_WARNINGS

#include<bangtal.h>

#include<stdio.h>
#include<stdlib.h>
#include<time.h>


SceneID scene1;
ObjectID player1, player2, player3;
TimerID timer1, timer2, timer3;



int speed = 0;
int speed1 = 0;
int speed2 = 0;
int x = 600;
int x1 = 300;
int x2 = 900;

int foesspeed = 1;

int score = 0;

ObjectID foes[100];
int foesX[100];
int foesY[100];
float foesSize[100];

ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown = true) {
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);
	if (shown) showObject(object);

	return object;
}


void createFoes() {
	char buf[50];
	for (int i = 0; i < 100; i++) {
		
		sprintf(buf, "box1.png", (i % 4) + 1);
		
		foesX[i] = rand() % 1280;
		foesY[i] = (rand() % 100 )* i + 720;
		foesSize[i] = 1.f / (rand() % 10 + 1);

		foes[i] = createObject(buf, scene1, foesX[i], foesY[i]);
		scaleObject(foes[i], foesSize[i]);


	}
}

void foesmove() {
	for (int i = 0; i < 100; i++) {
		foesY[i] -= foesspeed;
		locateObject(foes[i], scene1, foesX[i], foesY[i]);
		
		if (foesY[i] < -200 * foesSize[i]) {
			score++;
			foesspeed = score / 100 + 1;

			foesX[i] = rand() % 1280;
			foesY[i] = (rand() % (1000 - score)) + 1000;
			foesSize[i] = 1.f / (rand() % 10 + 1);

			locateObject(foes[i], scene1, foesX[i], foesY[i]);
			
		}
	}

}



bool pointInRect(int x, int y, int rx, int ry, int size) {
	if (x < rx) return false;
	if (x > rx + size) return false;
	if (y < ry) return false;
	if (y > ry + size) return false;

	return true;
}

bool collided(int x, int y, int rx, int ry, int size) {
	return pointInRect(x + 10, y + 20, rx, ry, size)
		|| pointInRect(x + 40, y + 20, rx, ry, size)
		|| pointInRect(x + 25, y + 45, rx, ry, size);

}

bool checkCollision() {
	for (int i = 0; i < 100; i++){
		if (collided(x,0,foesX[i],foesY[i],foesSize[i]*200)) return true;

	}
	return false;
}




void keyboardCallback(KeyCode code, KeyState state) {
	if (state == KeyState::KEY_PRESSED) {
		if (code == KeyCode::KEY_LEFT_ARROW) speed--;
		else if (code == KeyCode::KEY_RIGHT_ARROW)speed++;
		else if (code == KeyCode::KEY_DOWN_ARROW) speed1--;
		else if (code == KeyCode::KEY_UP_ARROW)speed1++;
		else if (code == KeyCode::KEY_F5) speed2--;
		else if (code == KeyCode::KEY_F6)speed2++;

	}

}

void player1move() {
	x += speed;
	if (x < 0) x = 0;
	if (x > 1230) x = 1230;
	locateObject(player1, scene1, x, 0);
	
	
}
void player2move() {

	x1 += speed1;
	if (x1 < 0) x1 = 0;
	if (x1 > 1230) x1 = 1230;
	locateObject(player2, scene1, x1, 0);
}
void player3move() {
	x2 += speed2;
	if (x2 < 0) x2 = 0;
	if (x2 > 1230) x2 = 1230;
	locateObject(player3, scene1, x2, 0);
}

void timerCallback(TimerID timer) {
	



	player1move();
	player2move();
	player3move();
	foesmove();


	if (checkCollision()) {
		char buf[256];
		sprintf(buf, "끝임\n점수: %d", score);
		showMessage(buf);
	}


	else {
		setTimer(timer, 0.01f);
		startTimer(timer);

	}

}

int main() {
	srand((unsigned int)time(NULL));


	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);


	setKeyboardCallback(keyboardCallback);
	setTimerCallback(timerCallback);

	scene1 = createScene("scene1", "배경.png");

	player1 = createObject("player1.png", scene1, 600, 0);
	player2 = createObject("player2.png", scene1, 300, 0);
	player3 = createObject("player3.png", scene1, 900, 0);

	timer1 = createTimer(0.01f);
	startTimer(timer1);

	timer2 = createTimer(0.01f);
	startTimer(timer2);

	timer3 = createTimer(0.01f);
	startTimer(timer3);


	createFoes();
	startGame(scene1);



}

