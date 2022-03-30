#include <SDL2/SDL_gamecontroller.h>
#include <unordered_map>
#include <chrono>
#include <stdlib.h>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Input.hpp"
#include "GameObject.hpp"
#include "Player.hpp"
#include "Stage.hpp"
#include "Avigunner.hpp"
#include "Vanshlicer.hpp"

using namespace std;

struct GamePad {
	bool buttons[SDL_CONTROLLER_BUTTON_MAX];
	short axis[SDL_CONTROLLER_AXIS_MAX];
};

inline void printInputs(Inputs& input) {
	// LMAO ALL OF ITS SHIT DOESSN"T EXIST
}

void applyFrame(vector<GameObject*>& gameobjs, Stage* stage, Inputs& player1, Inputs& player2) {
	/*
	cout << "PLAYER 1" << endl;
	printInputs(player1);
	cout << "PLAYER 2" << endl;
	printInputs(player2);
	*/
	static_cast<Player*>(gameobjs.at(0))->applyFrame(gameobjs, stage, player1);
	static_cast<Player*>(gameobjs.at(1))->applyFrame(gameobjs, stage, player2);
	for (int x = 0; x < gameobjs.size(); x++) {
//		cout << "X: " << x << endl;
		bool dead = gameobjs.at(x)->draw(gameobjs);
		if (dead) {
//			cout << "death\n";
			delete gameobjs.at(x);
			gameobjs.erase(gameobjs.begin() + x);
			x--;
		}
	}
//	cout << "endl\n";
}

void localInputs(Player* player, const GamePad& controllerInput, const GamePad& lastControllerInput) {
	Inputs thisFrame;
	float xaxis = controllerInput.axis[SDL_CONTROLLER_AXIS_LEFTX] / 32768.0;
	float yaxis = controllerInput.axis[SDL_CONTROLLER_AXIS_LEFTY] / 32768.0;
//	cout << xaxis << endl;

	if (xaxis == 0) {
		if (yaxis >= 0) {
			// cout << "up" << endl;
			thisFrame.direction.angle = M_PI / -2.0;
		}
		else {
			// cout << "down" << endl;
			thisFrame.direction.angle = M_PI / 2.0;			
		}
	}
	else if (xaxis > 0) {
		thisFrame.direction.angle = -atan(yaxis / xaxis);
	}
	else {
		thisFrame.direction.angle = M_PI - atan(yaxis / xaxis);
	}


	thisFrame.direction.magnitude = sqrt(xaxis * xaxis + yaxis * yaxis);

	if (controllerInput.buttons[SDL_CONTROLLER_BUTTON_A] && !lastControllerInput.buttons[SDL_CONTROLLER_BUTTON_A]) {
		thisFrame.quick = true;
	}
	if (controllerInput.buttons[SDL_CONTROLLER_BUTTON_B]) {
		thisFrame.charge = true;
	}
	if (controllerInput.buttons[SDL_CONTROLLER_BUTTON_X] || controllerInput.buttons[SDL_CONTROLLER_BUTTON_Y]) {
		thisFrame.jump = true;
		if (!lastControllerInput.buttons[SDL_CONTROLLER_BUTTON_X] && !lastControllerInput.buttons[SDL_CONTROLLER_BUTTON_Y]) {
			thisFrame.jumpPressed = true;
		}
	}

	player->inputQueue.push_back(thisFrame);
}

void getInputs(Player* player, const vector<GamePad>& controllerInputs, const vector<GamePad>& lastControllerInputs) {
	switch (player->inputs.method) {
		case ONLINE:
			// CHECK THREAD FOR INFO
			break;
		case KEYBOARD:
			localInputs(player, controllerInputs.back(), lastControllerInputs.back());
			break;
		case CONTROLLER:
			localInputs(player, controllerInputs[player->inputs.controllerNum], lastControllerInputs[player->inputs.controllerNum]);
			break;
	}
}

// COULD LEAD TO POTENTIAL BULLSHIT
// CURRENTLY A MEM LEAK
// ALSO CURRENTLY BROKEN RIP
void deepCopy(vector<GameObject*>& vecFrom, vector<GameObject*>& vecTo) {
	while (!vecTo.empty()) {
		delete vecTo.back();
		vecTo.pop_back();
	}

	for (GameObject* ep : vecFrom) {
		Player* temp = new Player();
		*temp = *(Player*) ep;
		vecTo.push_back(temp);
	}
}

void runGame() {
	vector<SDL_GameController*> connectedControllers;
	vector<GamePad> controllerInputs;
	vector<GamePad> lastControllerInputs;
	int numGamepads;
	const int FPS = 60;

	unordered_map<SDL_Keycode, SDL_GameControllerButton> keymap = {
		{SDLK_LSHIFT, SDL_CONTROLLER_BUTTON_LEFTSTICK}, 
		{SDLK_SPACE, SDL_CONTROLLER_BUTTON_START}, 
		{SDLK_g, SDL_CONTROLLER_BUTTON_B}, 
		{SDLK_f, SDL_CONTROLLER_BUTTON_A}, 
		{SDLK_h, SDL_CONTROLLER_BUTTON_X}, 
	};
	if (SDL_Init(SDL_INIT_VIDEO) > 0) {
		cout << "SDL Video Failure: " << SDL_GetError() << "\n";
	}
	if (!IMG_Init(IMG_INIT_PNG)) {
		cout << "IMG PNG Failure: " << SDL_GetError() << "\n";
	}
	if (SDL_WasInit(SDL_INIT_GAMECONTROLLER) != 1) {
		SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);
	}
//	cout << "TTF\n";
	if(TTF_Init() == -1) {
	    cout << "TTF_Init: " << TTF_GetError() << endl;
	}
//	cout << "TWO\n";

	int nJoysticks = SDL_NumJoysticks();
	numGamepads = 0;

	// Count how many controllers there are
	for (int i = 0; i < nJoysticks; i++)
		if (SDL_IsGameController(i))
			numGamepads++;
			
	// If we have some controllers attached
	if (numGamepads > 0) {
		for (int i = 0; i < numGamepads; i++) {
			// Open the controller and add it to our list
			SDL_GameController* pad = SDL_GameControllerOpen(i);
			if (SDL_GameControllerGetAttached(pad) == 1) {
				connectedControllers.push_back(pad);
			}
			else {
				cout << "SDL_GetError() = " << SDL_GetError() << endl;
			}
		}
		SDL_GameControllerEventState(SDL_ENABLE);
	}

	// Vectors are empty to begin with, this sets their size
	controllerInputs.resize(numGamepads + 1);
	lastControllerInputs.resize(numGamepads + 1);

	// Set the status of the controllers to "nothing is happening"
	for (int i = 0; i < numGamepads; i++) {
		for (int a = 0; a < SDL_CONTROLLER_AXIS_MAX; a++) {
			controllerInputs[i].axis[a] = 0;
			lastControllerInputs[i].axis[a] = 0;
		}
		for (int b = 0; b < SDL_CONTROLLER_BUTTON_MAX; b++) {
			controllerInputs[i].buttons[b] = false;
			lastControllerInputs[i].buttons[b] = false;
		}
	}


	RenderWindow window("Ultra Sea Wars Annihilation", 1280, 720);
	bool gameRunning = true;
	bool gameplaying = true;
	SDL_Event event;

	Entity* background = new Entity(0, 0, window.loadTexture("res/gfx/back.png"), 1280, 720);
	Stage* stage = new Stage(Circle(600, 500, 1000), window.loadTexture("res/gfx/ScuBattlefield.png"));

	vector<GameObject*> gameobjs = {new Avigunner(window, PLAYER1), new Vanshlicer(window, PLAYER2)}; // INITIALIZE HERE (FIRST TWO ELEMENTS ARE ALWAYS PLAYERS)
	vector<GameObject*> rollbackpoint;
	deepCopy(gameobjs, rollbackpoint);

	cout << numGamepads << endl;

	while (gameRunning) {
		auto start = chrono::steady_clock().now();

//		cout << "Frame starter\n";

		window.clear();
		window.render(background, true);

		for (int i = 0; i <= numGamepads; i++) {
			for (int a = 0; a < SDL_CONTROLLER_AXIS_MAX; a++) {
				lastControllerInputs[i].axis[a] = controllerInputs[i].axis[a];
			}
			for (int b = 0; b < SDL_CONTROLLER_BUTTON_MAX; b++) {
				lastControllerInputs[i].buttons[b] = controllerInputs[i].buttons[b];
			}
		}

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
//				cout << "HELLO" << endl;
				gameRunning = false;
			}

			SDL_Keycode kc = event.key.keysym.sym;
			/* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
			switch( event.type ){
				case SDL_KEYDOWN:
					if (keymap.find(kc) != keymap.end()) {
						controllerInputs[numGamepads].buttons[keymap[kc]] = true;
					}
					/*
					if (kc == SDLK_t) {
						cout << "Point back roll" << endl;
						deepCopy(gameobjs, rollbackpoint);
					}
					if (kc == SDLK_r) {
						cout << "RBP" << endl;
						deepCopy(rollbackpoint, gameobjs);
					}
					*/
					if (kc == SDLK_w) {
						controllerInputs[numGamepads].axis[SDL_CONTROLLER_AXIS_LEFTY] = -25000;
					}
					if (kc == SDLK_s) {
						controllerInputs[numGamepads].axis[SDL_CONTROLLER_AXIS_LEFTY] = 25000;
					}
					if (kc == SDLK_a) {
//						cout << "a\n"; 
						controllerInputs[numGamepads].axis[SDL_CONTROLLER_AXIS_LEFTX] = -25000;
					}
					if (kc == SDLK_d) {
//						cout << "d\n"; 
						controllerInputs[numGamepads].axis[SDL_CONTROLLER_AXIS_LEFTX] = 25000;
					}
					break;

				case SDL_KEYUP:
					if (keymap.find(kc) != keymap.end()) {
						controllerInputs[numGamepads].buttons[keymap[kc]] = false;
					}
					if (kc == SDLK_w && controllerInputs[numGamepads].axis[SDL_CONTROLLER_AXIS_LEFTY] < 0) {
						controllerInputs[numGamepads].axis[SDL_CONTROLLER_AXIS_LEFTY] = 0;
					}
					if (kc == SDLK_s && controllerInputs[numGamepads].axis[SDL_CONTROLLER_AXIS_LEFTY] > 0) {
						controllerInputs[numGamepads].axis[SDL_CONTROLLER_AXIS_LEFTY] = 0;
					}
					if (kc == SDLK_a && controllerInputs[numGamepads].axis[SDL_CONTROLLER_AXIS_LEFTX] < 0) {
						controllerInputs[numGamepads].axis[SDL_CONTROLLER_AXIS_LEFTX] = 0;
					}
					if (kc == SDLK_d && controllerInputs[numGamepads].axis[SDL_CONTROLLER_AXIS_LEFTX] > 0) {
						controllerInputs[numGamepads].axis[SDL_CONTROLLER_AXIS_LEFTX] = 0;
					}
					break;
/*
				// This happens when a device is added
				// A future improvement is to cope with new controllers being plugged in
				// when the game is running
				case SDL_CONTROLLERDEVICEADDED:
					numGamepads++;
					controllerInputs.resize(numGamepads + 1);
					lastControllerInputs.resize(numGamepads + 1);
//					cout << "DEVICEADDED cdevice.which = " << event.cdevice.which << endl;
					break;

				case SDL_CONTROLLERDEVICEREMOVED:
					numGamepads--;
					controllerInputs.resize(numGamepads + 1);
					lastControllerInputs.resize(numGamepads + 1);
//					cout << "DEVICEADDED cdevice.which = " << event.cdevice.which << endl;
					break;
*/
				case SDL_CONTROLLERBUTTONDOWN:
					// Cycle through the controllers
					for (int i = 0; i < numGamepads; i++) {
						// Looking for the button that was pressed
						if (event.cbutton.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(connectedControllers[i]))) {
							// So the relevant state can be updated
							controllerInputs[i].buttons[event.cbutton.button] = true;
						}
					}
					break;
					
				// Do the same for releasing a button
				case SDL_CONTROLLERBUTTONUP:
					for (int i = 0; i < numGamepads; i++) {
						if (event.cbutton.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(connectedControllers[i]))) {
							cout << "Gamepad: " << i << endl;
							controllerInputs[i].buttons[event.cbutton.button] = false;
						}
					}
					break;

				// And something similar for axis motion
				case SDL_CONTROLLERAXISMOTION:
					for (int i = 0; i < numGamepads; i++) {
						if (event.cbutton.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(connectedControllers[i]))) {
							/*
							cout << "Gamepad: " << i << endl;
							cout << (int) event.caxis.axis << endl;
							cout << event.caxis.value << endl;
							*/
							controllerInputs[i].axis[event.caxis.axis] = event.caxis.value;
						}
					}
					break;

				default:
					break;
			}
		}

		if (gameplaying) {
			Player* player1 = static_cast<Player*>(gameobjs.at(0));
			Player* player2 = static_cast<Player*>(gameobjs.at(1));
			list<Inputs>& player1Queue = player1->inputQueue;
			list<Inputs>& player2Queue = player2->inputQueue;
			list<Inputs>& player1Rollback = player1->rollback;
			list<Inputs>& player2Rollback = player2->rollback;
			InputMethod& p1input = player1->inputs;
			InputMethod& p2input = player2->inputs;

			// GET INPUTS
			for (int i = 0; i <= numGamepads; i++) {
				if (controllerInputs[i].buttons[SDL_CONTROLLER_BUTTON_START]) {
					if (p1input.method != ONLINE && p2input.method != ONLINE && (p1input.method != CONTROLLER || p1input.controllerNum != i)) {
						p2input = p1input;
						if (i == numGamepads) {
							p1input.method = KEYBOARD;
						}
						else {
							p1input.method = CONTROLLER;
							p1input.controllerNum = i;
						}
					}
				}
			}
			if (controllerInputs[numGamepads].buttons[SDL_CONTROLLER_BUTTON_LEFTSTICK]) {
				if (abs(controllerInputs[numGamepads].axis[SDL_CONTROLLER_AXIS_LEFTX]) > 20000) {
					controllerInputs[numGamepads].axis[SDL_CONTROLLER_AXIS_LEFTX] /= 2;
				}
				if (abs(controllerInputs[numGamepads].axis[SDL_CONTROLLER_AXIS_LEFTY]) > 20000) {
					controllerInputs[numGamepads].axis[SDL_CONTROLLER_AXIS_LEFTY] /= 2;
				}
			}


			getInputs(player1, controllerInputs, lastControllerInputs);
			getInputs(player2, controllerInputs, lastControllerInputs);

			// ROLLBACK AND INPUT USEAGE CODE
			while (player1Queue.size() > 1 && player2Queue.size() > 1 && player1Rollback.size() == 0 && player2Rollback.size() == 0) {
				player1Queue.pop_front(); // Pop first because first input was the last input used
				player2Queue.pop_front();
				applyFrame(gameobjs, stage, player1Queue.front(), player2Queue.front());
//				applyFrame(rollbackpoint, stage, player1Queue.front(), player2Queue.front());
			}
			if (player1Queue.size() > 1 || player2Queue.size() > 1) {
				cout << "ROLLBACK BS ALERT" << endl;

				while (player1Queue.size() > 1) {
					player1Queue.pop_front();
					player1Rollback.push_back(player1Queue.front());
					applyFrame(gameobjs, stage, player1Queue.front(), player2Queue.front());
				}
				while (player2Queue.size() > 1) {
					player2Queue.pop_front();
					player2Rollback.push_back(player2Queue.front());
					applyFrame(gameobjs, stage, player1Queue.front(), player2Queue.front());
				}

				bool done = false;
				while (player1Rollback.size() > 0 && player2Rollback.size() > 0) {
					applyFrame(rollbackpoint, stage, player1Rollback.front(), player2Rollback.front());
					player1Rollback.pop_front();
					player2Rollback.pop_front();
					done = true;
				}
				if (done) {
					deepCopy(rollbackpoint, gameobjs);
					for (Inputs& i : player1Rollback) {
						applyFrame(gameobjs, stage, i, player2Queue.front());
					}
					for (Inputs& i : player2Rollback) {
						applyFrame(gameobjs, stage, player1Queue.front(), i);
					}
				}
			}

			// --- CAMERA ---
			window.x = 2 * min(player1->x, player2->x) - max(player1->x + player1->show_width, player2->x + player2->show_width);
			window.y = 2 * min(player1->y, player2->y) - max(player1->y + player1->show_height, player2->y + player2->show_height);
			window.zoom = 300.0 /  max(abs(player1->x - player2->x), abs(player1->y - player2->y));
			/*
			SDL_Rect actualPlaces[2] = {window.getDestRect(player1), window.getDestRect(player2)};
			for (SDL_Rect& r : actualPlaces) {
				if (r.x < 200) {
					window.x -= sqrt(200 - r.x);
					window.zoom -= window.zoom / 1000.0;
				}
				else if (r.x > 1000) {
					window.x += sqrt(r.x - 1000);
					window.zoom -= window.zoom / 1000.0;					
				}
				else {
					window.zoom += window.zoom / 10000.0;
				}
				if (r.y < 200) {
					window.y -= sqrt(200 - r.y);
					window.zoom -= window.zoom / 1000.0;
				}
				else if (r.y > 1000) {
					window.y += sqrt(r.y - 1000);
					window.zoom -= window.zoom / 1000.0;					
				}
				else {
					window.zoom += window.zoom / 10000.0;
				}
				if ((r.x < 0) || (r.x + r.w > 1200) || (r.y < 0) || (r.y + r.h > 1200)) {
					window.zoom -= window.zoom / 500.0;	
				}
			}
			*/
			if (window.zoom < 0.5) {
				window.zoom = 0.5;
			}
			if (window.zoom > 1) {
				window.zoom = 1;
			}
			if (abs(window.x) > 900) {
				window.x = 900 * abs(window.x) / window.x;
			}
			if (abs(window.y) > 600) {
				window.y = 600 * abs(window.y) / window.y;
			}
			

			// --- RENDERING ---

			window.render(stage);
			for (GameObject* e : gameobjs) {
				window.render(e);
			}

/*
			// HITBOXES
			for (Circle& c : player1->hitboxes) {
				if (c.attack) {
					c.draw(&window, 255, 0, 0, 100);
				}
				else {
					c.draw(&window, 255, 255, 0, 100);
				}
			}
			for (Circle& c : player2->hitboxes) {
				if (c.attack) {
					c.draw(&window, 255, 0, 0, 100);
				}
				else {
					c.draw(&window, 255, 255, 0, 100);
				}
			}
*/
//			stage->collide.draw(&window, 255, 255, 0, 100);

//			cout << "BEEF\n";
			window.drawText(to_string((int) player1->percent) + "%", 255, 255, 255, 255, 0, 0, 100, 100);
			window.drawText(to_string((int) player2->percent) + "%", 255, 255, 255, 255, 1100, 0, 100, 100);
//			cout << "AFTER\n";
		}
		window.display();

//		cout << "Frame ender\n";

		auto end = chrono::steady_clock().now();
		chrono::duration<double> frameDone = end - start;

//		cout << (1.0 / FPS) << endl;
//		cout << frameDone.count() << endl;
		double delay = 1000 * ((1.0 / FPS) - frameDone.count());
//		cout << delay << endl;

		if (delay > 0) {
			SDL_Delay(delay);
		}

		/*
		end = chrono::steady_clock().now();
		frameDone = end - start;
		cout << "Frame done in " << frameDone.count() << " seconds.\n";
		*/
	}

	window.cleanUp();
	SDL_Quit();
}

int main(int argc, char *args[]) {
	runGame();
	return 0;
}