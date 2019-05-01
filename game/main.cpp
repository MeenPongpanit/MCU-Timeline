/*Computer Programing Project Game MCU timeline.*/
#include <stdio.h>
#include <stdlib.h> 
#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <time.h>     

using namespace std;

//Screen size
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;

//Texture wrapper class
class LTexture
{
public:
	LTexture();
	~LTexture();
	bool loadFromFile(std::string path);
	void free();
	void render(int x, int y);
	int getWidth();
	int getHeight();

private:
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
};

//Starts up SDL and creates window
bool init();

//Loads image function
bool loadMedia1(); // main game background and deck
bool loadMedia2(); // top deck reveal
bool loadMedia3(); // poll top part
bool loadMedia4(); // poll bottom part
bool loadMedia5(); // description
bool loadMedia6(); // other background
bool loadMedia7(); // score

//Frees media and shuts down SDL
void close();

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

//image variable
LTexture gFooTexture;
LTexture g_card_reveal;
LTexture g_card_place_1;
LTexture g_card_place_2;
LTexture gBackgroundTexture;
LTexture g_des;
LTexture g_page;
LTexture scr_get;
LTexture g_slash;
LTexture g_max;



LTexture::LTexture()
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	free();
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}
	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render(int x, int y)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	SDL_RenderCopy(gRenderer, mTexture, NULL, &renderQuad);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("MCU Timeline.", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}



bool loadMedia1(string file1) // background and deck
{
	bool success = true;

	if (!gBackgroundTexture.loadFromFile("res/back.png"))
	{
		printf("Failed to load background background image!\n");
		success = false;
	}
	if (!gFooTexture.loadFromFile(file1))
	{
		printf("Failed to load Foo' foo image!\n");
		success = false;
	}

	return success;
}

bool loadMedia2(string file1) // top deck
{
	bool success = true;

	if (!g_card_reveal.loadFromFile(file1))
	{
		printf("Failed to load Foo' reveal image!\n");
		success = false;
	}

	return success;
}

bool loadMedia3(string file1) // reveal card top
{
	bool success = true;

	if (!g_card_place_1.loadFromFile(file1))
	{
		printf("Failed to load Foo' place_1 image!\n");
		success = false;
	}

	return success;
}

bool loadMedia4(string file1) // reveal card bottom
{
	bool success = true;

	if (!g_card_place_2.loadFromFile(file1))
	{
		printf("Failed to load place_2' place image!\n");
		success = false;
	}

	return success;
}

bool loadMedia5(string file1) // description
{
	bool success = true;

	if (!g_des.loadFromFile(file1))
	{
		printf("Failed to load descrip' place image!\n");
		success = false;
	}

	return success;
}

bool loadMedia6(string file1) // other bg
{
	bool success = true;

	if (!g_page.loadFromFile(file1))
	{
		printf("Failed to load page image!\n");
		success = false;
	}

	return success;
}

bool loadMedia7(string file1, string file2, string file3) // score slash max
{
	bool success = true;

	if (!scr_get.loadFromFile(file1))
	{
		printf("Failed to load scr get image!\n");
		success = false;
	}
	if (!g_slash.loadFromFile(file2))
	{
		printf("Failed to load slash image!\n");
		success = false;
	}
	if (!g_max.loadFromFile(file3))
	{
		printf("Failed to load max scr image!\n");
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded images
	gFooTexture.free();
	g_card_reveal.free();
	g_card_place_1.free();
	g_card_place_2.free();
	gBackgroundTexture.free();
	g_des.free();
	g_page.free();
	scr_get.free();
	g_slash.free();
	g_max.free();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

typedef struct {

	string pic_name;
	int seq;
	string des_name = "NULL";
	string deck = "NULL";

}CARD;

void show_img() {
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);
	//Render img texture to screen
	gBackgroundTexture.render(0, 0);
	gFooTexture.render(90, 170);
	g_card_reveal.render(359, 170);
	g_card_place_1.render(766, 15);
	g_card_place_2.render(766, 327);
	//Update screen
	SDL_RenderPresent(gRenderer);
}

void play() {
	srand(time(NULL));

	CARD file[31];
	CARD temp;

	bool play_again_stuatus = false;
	bool quit = false;
	

	int num1, num2;
	int i, j = 0, k = 0, m = 0;
	int check = 0;
	int block_action = -1;
	int score = -1;
	int score_past = 0;

	string ans;
	string you_select;
	string img;
	string result;
	string card_place[30];
	string collect;
	string collect2;

	// maybe this faster ??
	string file_name[30] = { "res/1.png",  "res/2.png",  "res/3.png", "res/4.png", "res/5.png", "res/6.png", "res/7.png",
					 "res/8.png", "res/9.png", "res/10.png", "res/11.png", "res/12.png", "res/13.png", "res/14.png", "res/15.png",
	 "res/16.png" , "res/17.png" , "res/18.png" , "res/19.png" , "res/20.png" , "res/21.png" , "res/22.png" , "res/23.png" , "res/24.png"
	, "res/25.png" , "res/26.png" , "res/27.png" , "res/28.png" , "res/29.png" , "res/30.png" };
	string file_des_name[30] = { "res/D1.png",  "res/D2.png",  "res/D3.png", "res/D4.png", "res/D5.png", "res/D6.png", "res/D7.png",
					 "res/D8.png", "res/D9.png", "res/D10.png", "res/D11.png", "res/D12.png", "res/D13.png", "res/D14.png", "res/D15.png",
	 "res/D16.png" , "res/D17.png" , "res/D18.png" , "res/D19.png" , "res/D20.png" , "res/D21.png" , "res/D22.png" , "res/D23.png" , "res/D24.png"
	, "res/D25.png" , "res/D26.png" , "res/D27.png" , "res/D28.png" , "res/D29.png" , "res/D30.png" };

	for (i = 0;i < 30;i++) {
		file[i].pic_name = file_name[i];
		file[i].des_name = file_des_name[i];
		file[i].seq = i;
	}
	file[30].seq = 30;


	//random 30 numbers

	for (i = 0;i < 200;i++) {
		num1 = rand() % 30;
		num2 = rand() % 30;
		temp = file[num1];
		file[num1] = file[num2];
		file[num2] = temp;
	}

	i = 0;

	init(); //Start up SDL and create window

	//loading LOGO and tutorial.
	loadMedia6("res/LOGO.png");
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);
	g_page.render(0, 0);
	SDL_RenderPresent(gRenderer);
	SDL_Delay(1500);
	loadMedia6("res/tutorial.png");
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);
	g_page.render(0, 0);
	SDL_RenderPresent(gRenderer);
	
	SDL_Event e; //Event handler

	//While application is running
	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				//Select surfaces based on key press
				
				switch (e.key.keysym.sym)
				{
					case SDLK_UP:

						if (collect != "res/blankcard.png" && collect != "res/1.png" && block_action > 0 && i > 1) {
							loadMedia4(collect);
							collect2 = collect;
							you_select = collect;

							for (m = 0;m < 30;m++) {
								if (collect == file[m].pic_name) {
									num2 = file[m].seq;
									break;
								}
							}

							for (m = num2 - 1;m > -1;m--) {
								if (file[m].deck != "NULL") {
									loadMedia3(file[m].deck);
									collect = file[m].deck;
									break;
								}

								if (m == 0) {
									loadMedia3("res/blankcard.png");
									collect = "res/blankcard.png";
								}
							}
							show_img();
						}

						
						break;

					case SDLK_DOWN:
						if (collect2 != "res/blankcard.png" && collect2 != "res/30.png" && block_action > 0 && i > 1) {
							loadMedia3(collect2);
							collect = collect2;
							for (m = 0;m < 30;m++) {
								if (collect2 == file[m].pic_name) {
									num1 = file[m].seq;
									break;
								}
							}

							for (m = num1 + 1;m < 30;m++) {
								if (file[m].deck != "NULL") {
									loadMedia4(file[m].deck);
									collect2 = file[m].deck;
									you_select = file[m].deck;
									break;
								}
								if (m == 29) {
									loadMedia4("res/blankcard.png");
									collect2 = "res/blankcard.png";
									you_select = "res/blankcard.png";
								}
							}
							show_img();
						}
						break;

					case SDLK_SPACE:
						
						if (i == 0) {
							block_action += 1;
							you_select = "res/blankcard.png";
							ans = "res/blankcard.png";
						}
						if (play_again_stuatus) {
							close();
							play_again_stuatus = false;
							play();
						}
						if (i != 31) {
							if (check == 0) {
								check = 1;
								block_action += 1;
								if (i > 0) {
									img = "";
									img += string("res/") + to_string(file[i - 1].seq + 1) + string(".png");
									file[file[i - 1].seq].deck = img;
								}
								if (i > 0) {
									for (j = file[i - 1].seq + 1;j < 31;j++) {
										if (file[j].deck != "NULL") {
											loadMedia4(file[j].deck);
											collect2 = file[j].deck;
											ans = file[j].deck;
											break;
										}
										if (j == 30) {
											loadMedia4("res/blankcard.png");
											collect2 = "res/blankcard.png";
											ans = "res/blankcard.png";
										}
									}
								}
								if (i <= 30) {
									if (i < 30) {
										loadMedia1("res/card.png");
										loadMedia2(file[i].pic_name);
									}
									else{
										loadMedia1("res/card.png");
										loadMedia2("res/blankcard.png");
									}
									if (i > 0 && i < 29) {
										loadMedia1("res/card.png");
										loadMedia3(file[i - 1].pic_name);
										loadMedia2(file[i].pic_name);
										collect = file[i - 1].pic_name;
									}
									else if (i == 29) {
										loadMedia3(file[i - 1].pic_name);
										loadMedia2(file[i].pic_name);
										loadMedia1("res/blankcard.png");
										collect = file[i - 1].pic_name;
									}
									else if (i == 0) {
										loadMedia1("res/card.png");
										loadMedia3("res/blankcard.png");
										loadMedia2(file[0].pic_name);
										collect = "res/card.png";
									}
									else {
										loadMedia1("res/blankcard.png");
										loadMedia3(file[29].pic_name);
										loadMedia2("res/blankcard.png");
										collect = file[29].pic_name;
									}
									show_img();
									if (i <= 30) {
										i += 1;
									}
								}
								if (ans == you_select || i == 2) {
									score += 1;
									if (score > 0) {
										loadMedia6("res/Correct.png");
										SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
										SDL_RenderClear(gRenderer);
										//Render img texture to screen
										gBackgroundTexture.render(0, 0);
										gFooTexture.render(90, 170);
										g_card_reveal.render(359, 170);
										g_card_place_1.render(766, 15);
										g_card_place_2.render(766, 327);
										g_page.render(0, 500);
										//Update screen
										SDL_RenderPresent(gRenderer);
									}
								}
								else if (ans != you_select && i > 2){
									loadMedia6("res/Incorrect.png");
									SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
									SDL_RenderClear(gRenderer);
									//Render img texture to screen
									gBackgroundTexture.render(0, 0);
									gFooTexture.render(90, 170);
									g_card_reveal.render(359, 170);
									g_card_place_1.render(766, 15);
									g_card_place_2.render(766, 327);
									g_page.render(0, 500);
									//Update screen
									SDL_RenderPresent(gRenderer);
								}

							
							}
							else {
								block_action -= 1;
								check = 0;
								if (i > 0) {
									img = "";
									img += string("res/D") + to_string(file[i - 1].seq + 1) + string(".png");
									loadMedia5(img);
									SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
									SDL_RenderClear(gRenderer);
									gBackgroundTexture.render(0, 0);
									gFooTexture.render(90, 170);
									g_card_reveal.render(359, 170);
									g_card_place_1.render(766, 15);
									g_card_place_2.render(766, 327);
									g_des.render(200, 150);
									//Update screen
									SDL_RenderPresent(gRenderer);
									img = "";
	
									
								}
							}
							
						}
						else { 
							// end game result
							SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
							SDL_RenderClear(gRenderer);
							loadMedia6("res/background.png");
							g_page.render(0, 0);
							loadMedia6("res/result.png");
							g_page.render(200, 150);
							img = "";
							img += string("res/num") + to_string(score) + string(".png");
							loadMedia7(img, "res/slash.png", "res/num30y.png");
							scr_get.render(400, 250);
							g_slash.render(450, 250);
							g_max.render(510, 250);
							//Update screen
							SDL_RenderPresent(gRenderer);
							play_again_stuatus = true;
						}

						
						break;
					default:
						break;
				}
			}
		}
	}

	close();
}

int main(int argc, char* args[])
{
	play();

	return 0;
}