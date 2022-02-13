// Include the Arduboy2 library
#include <Arduboy2.h>

// Include TileType
#include "TileType.h"

// Include the tile sheet
#include "TileSheet.h"

// Arduboy2 Object
Arduboy2 arduboy;

// Tile Dimensions
constexpr uint8_t tileWidth = 16;
constexpr uint8_t tileHeight = 16;

// Map Dimensions (in tiles)
constexpr uint8_t mapWidth = 32;
constexpr uint8_t mapHeight = 32;

// Map Dimensions (in pixels)
constexpr uint16_t fullMapWidth = (mapWidth * tileWidth);
constexpr uint16_t fullMapHeight = (mapHeight * tileHeight);

// Tile Map
TileType tileMap[mapHeight][mapWidth] {};

// Camera Position
int16_t cameraX = (fullMapWidth / 2);
int16_t cameraY = (fullMapHeight / 2);

// Camera Boundaries
constexpr int16_t cameraXMinimum = 0;
constexpr int16_t cameraXMaximum = (fullMapWidth - WIDTH);
constexpr int16_t cameraYMinimum = 0;
constexpr int16_t cameraYMaximum = (fullMapHeight - HEIGHT);

// Fills the map with random tiles.
void generateMap()
{
	for(uint8_t tileY = 0; tileY < mapHeight; ++tileY)
		for(uint8_t tileX = 0; tileX < mapWidth; ++tileX)
			tileMap[tileY][tileX] = static_cast<TileType>(random() % 4);
}

void handleInput()
{
	// If the left button is pressed.
	if(arduboy.pressed(LEFT_BUTTON))
	{
		// If cameraX is greater than its allowed minimum.
		if(cameraX > cameraXMinimum)
			// Decrease cameraX by 1.
			--cameraX;
	}
	
	// If the right button is pressed.
	if(arduboy.pressed(RIGHT_BUTTON))
	{
		// If cameraX is less than its allowed maximum.
		if(cameraX < cameraXMaximum)
			// Increase cameraX by 1.
			++cameraX;
	}

	// If the up button is pressed.
	if(arduboy.pressed(UP_BUTTON))
	{
		// If cameraY is greater than its allowed minimum.
		if(cameraY > cameraYMinimum)
			// Decrease cameraY by 1.
			--cameraY;
	}
	
	// If the down button is pressed.
	if(arduboy.pressed(DOWN_BUTTON))
	{
		// If cameraY is less than its allowed maximum.
		if(cameraY < cameraYMaximum)
			// Increase cameraY by 1.
			++cameraY;
	}
}

void drawTileMap()
{
	for(uint8_t tileY = 0; tileY < mapHeight; ++tileY)
	{
		// Calculate the y position to draw the tile at.
		// (Making sure to factor in the camera.)
		int16_t drawY = ((tileY * tileHeight) - cameraY);

		// If the tile would be offscreen.
		if((drawY < -tileHeight) || (drawY > HEIGHT))
			// Skip this row and continue with the next row.
			continue;
		
		for(uint8_t tileX = 0; tileX < mapWidth; ++tileX)
		{
			// Calculate the x position to draw the tile at.
			// (Making sure to factor in the camera.)
			int16_t drawX = ((tileX * tileWidth) - cameraX);

			// If the tile would be offscreen.
			if((drawX < -tileWidth) || (drawX > WIDTH))
				// Skip this tile and continue with the next one.
				continue;

			// Read the tile from the map.
			TileType tileType = tileMap[tileY][tileX];

			// Figure out the tile index.
			uint8_t tileIndex = toTileIndex(tileType);

			// Draw the tile at the calculated position.
			Sprites::drawOverwrite(drawX, drawY, tileSheet, tileIndex);
		}
	}
}

// Setup code
// Runs once at the start.
void setup()
{
	// Arduboy2 begin function
	// Runs the intro logo and provides other features.
	arduboy.begin();

	// Randomly generate the map.
	generateMap();
}

// Game loop code
// Runs once per frame.
void loop()
{
	// If it's not time to draw the next frame...
	if(!arduboy.nextFrame())
		// Exit the function.
		return;

	// Update the button state.
	// (Detect buttons being pressed and released.)
	arduboy.pollButtons();

	// Respond to user input.
	handleInput();

	// Draw the tile map.
	drawTileMap();

	// Update the screen.
	arduboy.display();
}