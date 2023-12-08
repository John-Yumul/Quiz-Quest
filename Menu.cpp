#include <string>
#include <SFML/Graphics.hpp>
#include<iostream>
#include <vector>
#include "Button.h"
#include "sqlite/sqlite3.h"
#include "Player.h"
#include "Collider.h"
#include "Building.h"
#include "Plant.h"
#include "Enter.h"
#include "Pathway.h"
#include "Floor.h"
#include "TextBox.h"

using namespace std;
using namespace sf;
string GameID = "";
void floortwo(RenderWindow& window, View& view, Texture& platex);
void floorthreeroomone(RenderWindow& window, View& view, Texture& platex);

VideoMode desktop = VideoMode::getDesktopMode()
;
const float desc_x = desktop.width;
const float desc_y = desktop.height;

static const float view_heightx = 1920 / 2;  //range lang ng view
static const float view_heighty = 1080 / 2;


static int idCallback(void* data, int argc, char** argv, char** azColName) {
	int* count = static_cast<int*>(data);
	*count = std::atoi(argv[0]); // Assuming the count is in the first column

	return 0;
}

bool isColliding(const sf::FloatRect& box1, const sf::FloatRect& box2) {
	return box1.intersects(box2);
}//will check of two object is colliding

void resizeview(const RenderWindow& window, View& view) {
	float ratio = float(window.getSize().x / window.getSize().y);
	view.setSize(ratio * view_heightx, ratio * view_heighty);
}//thiss function will help us to make the character not stretch 
void posUpdate(float x, float y) {
	sqlite3* db;
	char* errMsg = nullptr;
	string syntax = "UPDATE PlayerTable SET PositionX = " + to_string(x) + ", PositionY = " + to_string(y) + " WHERE GameID = '" + GameID + "';";;
	// Open the SQLite database
	int rc = sqlite3_open("Unique.db", &db);

	if (rc) {
		std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		return;
	}

	// SQL command to update data
	const char* sql = syntax.c_str();

	// Execute the SQL command
	rc = sqlite3_exec(db, sql, nullptr, 0, &errMsg);

	if (rc != SQLITE_OK) {
		std::cerr << "SQL error: " << errMsg << std::endl;
		sqlite3_free(errMsg);
	}
	else {
		std::cout << "Update operation successful." << std::endl;
	}

	// Close the database connection
	sqlite3_close(db);
}
void InteractionGuard(RenderWindow& window) {
	Texture tex;
	tex.loadFromFile("Graphics/guard1.png");
	Plant Guard(&tex, Vector2f(window.getSize().x * 0.5, window.getSize().y * 0.85), Vector2f(window.getSize().x * 0.75, window.getSize().y * 0.5));
	Plant chatBox(nullptr, Vector2f(window.getSize().x, (window.getSize().y / 3)), Vector2f(window.getSize().x / 2, window.getSize().y * 0.80));
	chatBox.Transparent();
	View view(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
	window.setView(view);
	Font font;
	font.loadFromFile("arial.ttf");
	Text convo;
	TextBox tb(window, font, Vector2f(window.getSize().x / 5, window.getSize().y / 5), Vector2f((window.getSize().x / 5), (window.getSize().y / 5)));
	convo.setFont(font);
	convo.setCharacterSize(30);
	convo.setFillColor(Color::Black);
	convo.setPosition(Vector2f(window.getSize().x / 11, window.getSize().y * 0.80));
	string x;
	int secondsPassed = 0;//timer
	float accumulatedTime = 0.0f;
	//defining tick
	float deltaTime = 0.0f;
	Clock clock;


	while (window.isOpen()) {
		Event ev;// event for the window... 
		deltaTime = clock.restart().asSeconds();
		accumulatedTime += deltaTime;

		if (accumulatedTime >= 1.0f) {
			secondsPassed += static_cast<int>(std::floor(accumulatedTime));
			accumulatedTime = fmod(accumulatedTime, 1.0f); // Store remaining time

		}
		cout << secondsPassed << endl;
		while (window.pollEvent(ev)) { //eto yung nagdedetect kung clinick ba yung "x" sa window or nag resize, zoom , etc
			switch (ev.type) {
			case Event::Closed:
				window.close();
				break;
			}
		}
		if (secondsPassed < 5) {
			x = "Good morning spartan!";
		}

		else if (secondsPassed < 10) {
			x = "Can I see your ID?";
		}
		else if (secondsPassed < 15) {
			x = "Oh, you're new here... ";
		}

		else if (secondsPassed < 20)
		{
			x = "I see, I see...";
		}
		else if (secondsPassed < 25)
		{
			x = "go to ssc to have you register your ID";
		}
		else if (secondsPassed < 30)
		{

		}
		if (Keyboard::isKeyPressed(Keyboard::Space)) {
			if (secondsPassed < 5)
				secondsPassed = 5;
			else if (secondsPassed < 10)
				secondsPassed = 10;
			else if (secondsPassed < 15)
				secondsPassed = 15;
			else if (secondsPassed < 20)
				secondsPassed = 20;
			else if (secondsPassed < 25)
				secondsPassed = 25;
			else if (secondsPassed < 30)
				secondsPassed = 30;
			sleep(seconds(0.5));
		}
		if (secondsPassed > 30) {
			return;
		}
		window.display();
		window.clear(Color::Blue);

		Guard.Draw(window);
		chatBox.Draw(window);
		window.draw(convo);
		convo.setString(x);


	}
}
double getPositionX() {
	sqlite3* db;
	int result = sqlite3_open("Unique.db", &db); // Replace "YourDatabase.db" with your database file name

	if (result != SQLITE_OK) {
		std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
		return 0.0; // Return default value or handle error accordingly
	}

	// Query to retrieve PositionX value based on GameID
	std::string query = "SELECT PositionX FROM PlayerTable WHERE GameID = '"+GameID+"';";

	sqlite3_stmt* stmt;
	result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

	if (result != SQLITE_OK) {
		std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		return 0.0; // Return default value or handle error accordingly
	}

	// Bind the GameID parameter to the query
	sqlite3_bind_text(stmt, 1, GameID.c_str(), -1, SQLITE_STATIC);

	// Execute the query
	result = sqlite3_step(stmt);

	double positionX = 0.0; // Default value if not found

	if (result == SQLITE_ROW) {
		positionX = sqlite3_column_double(stmt, 0); // Get the PositionX value
	}

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return positionX;
}

// Function to retrieve PositionY based on GameID
double getPositionY() {
	sqlite3* db;
	int result = sqlite3_open("Unique.db", &db); // Replace "YourDatabase.db" with your database file name

	if (result != SQLITE_OK) {
		std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
		return 0.0; // Return default value or handle error accordingly
	}

	// Query to retrieve PositionY value based on GameID
	std::string query = "SELECT PositionY FROM PlayerTable WHERE GameID = '" + GameID + ";";

	sqlite3_stmt* stmt;
	result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

	if (result != SQLITE_OK) {
		std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		return 0.0; // Return default value or handle error accordingly
	}

	// Bind the GameID parameter to the query
	sqlite3_bind_text(stmt, 1, GameID.c_str(), -1, SQLITE_STATIC);

	// Execute the query
	result = sqlite3_step(stmt);

	double positionY = 0.0; // Default value if not found

	if (result == SQLITE_ROW) {
		positionY = sqlite3_column_double(stmt, 0); // Get the PositionY value

		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return positionY;
	}

}
void updateYear(string Year) {
	sqlite3* db;
	char* errMsg = nullptr;
	string syntax = "UPDATE PlayerTable SET Yearlvl = '" + Year + "' WHERE GameID = '" + GameID + "';";
	// Open the SQLite database
	int rc = sqlite3_open("Unique.db", &db);

	if (rc) {
		std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		return;
	}

	// SQL command to update data
	const char* sql = syntax.c_str();

	// Execute the SQL command
	rc = sqlite3_exec(db, sql, nullptr, 0, &errMsg);

	if (rc != SQLITE_OK) {
		std::cerr << "SQL error: " << errMsg << std::endl;
		sqlite3_free(errMsg);
	}
	else {
		std::cout << "Update operation successful." << std::endl;
	}

	// Close the database connection
	sqlite3_close(db);
}
char* getCharValueFromDB(const char* dbName, const char* query) {
	sqlite3* db;
	sqlite3_stmt* stmt;
	const char* sql;
	int rc;

	// Open database
	rc = sqlite3_open(dbName, &db);

	if (rc) {
		std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
		return nullptr;
	}

	// SQL query

	sql = query;

	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

	if (rc != SQLITE_OK) {
		std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		cout << "hindi gago";
		return nullptr;
	}

	// Execute the query
	rc = sqlite3_step(stmt);

	if (rc == SQLITE_ROW) {
		// Retrieve character value
		const char* charValue = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));

		// Duplicate the character value and return using _strdup() instead of strdup()
		char* result = _strdup(charValue); // Remember to free this memory after use
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		cout << "legit";
		return result;
	}

	// Finalize the statement and close the database
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	cout << "gago";
	return nullptr;
}
string Quest1(const string& syntax, int row, const string& Table) {
	sqlite3* db;
	sqlite3_stmt* stmt;
	const char* tail;

	string qry = "SELECT " + syntax + " FROM " + Table + " WHERE QuestionNumber =" + std::to_string(row) + ";";
	const char* dbName = "Unique.db";
	const char* query = qry.c_str();

	string valueFromDB; // Variable to store the retrieved value as string

	int rc = sqlite3_open(dbName, &db);

	if (rc) {
		cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
		return ""; // Return an empty string on error
	}

	rc = sqlite3_prepare_v2(db, query, -1, &stmt, &tail);

	if (rc != SQLITE_OK) {
		std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		return ""; // Return an empty string on error
	}

	rc = sqlite3_step(stmt);

	if (rc == SQLITE_ROW) {
		const unsigned char* textValue = sqlite3_column_text(stmt, 0);
		if (textValue) {
			valueFromDB = reinterpret_cast<const char*>(textValue);
			cout << "Value from the database: " << valueFromDB << std::endl;
		}
		else {
			cout << "Value is NULL" << std::endl;
		}
	}
	else {
		cout << "Row not found or value is NULL" << std::endl;
	}

	sqlite3_finalize(stmt);
	sqlite3_close(db);

	return valueFromDB; // Return the retrieved value
}
int game1(RenderWindow& window, int difficulty) {
	int rowTotal = 10 + difficulty;
	Color navyBlue = Color(0, 0, 128);

	string fiLoc = "Graphics/button.png";
	Button A(fiLoc, Vector2f(window.getSize().x * 0.33, 100), Vector2f(100, window.getSize().y * 0.6));
	Button B(fiLoc, Vector2f(window.getSize().x * 0.33, 100), Vector2f((window.getSize().x * 0.5) + 100, window.getSize().y * 0.6));
	Button C(fiLoc, Vector2f(window.getSize().x * 0.33, 100), Vector2f(100, window.getSize().y * 0.9));
	Button D(fiLoc, Vector2f(window.getSize().x * 0.33, 100), Vector2f((window.getSize().x * 0.5) + 100, window.getSize().y * 0.9));
	Text a, b, c, d, question;
	Texture tex;
	tex.loadFromFile("Graphics/chatbox.png");
	RectangleShape Question(Vector2f(750, 200));
	Question.setPosition(Vector2f(A.getpos().x - 125, 50));
	Question.setTexture(&tex);
	Font font;
	font.loadFromFile("pix.ttf");
	string astr, bstr, cstr, dstr;
	a.setFont(font);
	a.setCharacterSize(15);
	a.setFillColor(sf::Color::White);
	a.setPosition(Vector2f(A.getpos().x - (A.getsize().x * 0.4), A.getpos().y - (A.getsize().y * 0.15)));
	b.setFont(font);
	b.setCharacterSize(15);
	b.setFillColor(sf::Color::White);
	b.setPosition(Vector2f(B.getpos().x - (B.getsize().x * 0.4), B.getpos().y - (B.getsize().y * 0.15)));
	d.setFont(font);
	d.setCharacterSize(15);
	d.setFillColor(sf::Color::White);
	d.setPosition(Vector2f(D.getpos().x - (D.getsize().x * 0.4), D.getpos().y - (C.getsize().y * 0.15)));
	c.setFont(font);
	c.setCharacterSize(15);
	c.setFillColor(sf::Color::White);
	c.setPosition(Vector2f(C.getpos().x - (C.getsize().x * 0.4), C.getpos().y - (D.getsize().y * 0.15)));
	question.setFont(font);
	question.setCharacterSize(20);
	question.setFillColor(sf::Color::Black);
	question.setPosition(Vector2f(Question.getPosition().x + 50, Question.getPosition().y + (Question.getSize().y * 0.15)));
	string table = "Game1";
	string syntax = "GameQuestion";
	int row = 1 + difficulty;
	string x = Quest1(syntax, row, table);
	question.setString(x);

	syntax = "A";
	x = Quest1(syntax, row, table);
	a.setString(x);

	syntax = "B";
	x = Quest1(syntax, row, table);
	b.setString(x);

	syntax = "C";
	x = Quest1(syntax, row, table);
	c.setString(x);

	syntax = "D";
	x = Quest1(syntax, row, table);
	d.setString(x);
	size_t indexToInsert = 40;

	int score = 0;
	Vector2f mouse;
	View view(sf::FloatRect(-150, 0, window.getSize().x, window.getSize().y));
	window.setView(view);
	while (window.isOpen())
	{
		syntax = "A";
		x = Quest1(syntax, row, table);
		a.setString(x);

		syntax = "B";
		x = Quest1(syntax, row, table);
		b.setString(x);

		syntax = "C";
		x = Quest1(syntax, row, table);
		c.setString(x);

		syntax = "D";
		x = Quest1(syntax, row, table);
		d.setString(x);

		syntax = "GameQuestion";
		x = Quest1(syntax, row, table);
		if (x.length() > 40) {
			x.insert(indexToInsert, "\n");
		}
		question.setString(x);

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
		mouse = window.mapPixelToCoords(Mouse::getPosition(window));
		if (A.bounds().contains(mouse)) {
			A.Hover();
			if (Mouse::isButtonPressed(Mouse::Left)) {
				A.Click();
			}
		}
		else {
			A.Def();
		}
		if (B.bounds().contains(mouse)) {
			B.Hover();
			if (Mouse::isButtonPressed(Mouse::Left)) {
				B.Click();
			}
		}
		else {
			B.Def();
		}
		if (C.bounds().contains(mouse)) {
			C.Hover();
			if (Mouse::isButtonPressed(Mouse::Left)) {
				C.Click();
			}
		}
		else {
			C.Def();
		}
		if (D.bounds().contains(mouse)) {
			D.Hover();
			if (Mouse::isButtonPressed(Mouse::Left)) {
				D.Click();
			}
		}
		else {
			D.Def();
		}
		if (Mouse::isButtonPressed(Mouse::Left)) {
			if (A.bounds().contains(mouse)) {
				syntax = "Answer";
				if (a.getString() == Quest1(syntax, row, table)) {
					score++;
				}
				row++;
				sleep(seconds(0.5));
			}
			else if (B.bounds().contains(mouse)) {
				syntax = "Answer";
				if (b.getString() == Quest1(syntax, row, table)) {
					score++;
				}
				row++;
				sleep(seconds(0.5));
			}
			if (C.bounds().contains(mouse)) {
				syntax = "Answer";
				if (c.getString() == Quest1(syntax, row, table)) {
					score++;
				}
				row++;
				sleep(seconds(0.5));
			}
			if (D.bounds().contains(mouse)) {
				syntax = "Answer";
				if (d.getString() == Quest1(syntax, row, table)) {
					score++;
				}
				row++;
				sleep(seconds(0.5));
			}
		}


		if (row > rowTotal) {
			return score;
		}
		window.clear(navyBlue);
		A.Draw(window);
		B.Draw(window);
		C.Draw(window);
		D.Draw(window);
		window.draw(a);
		window.draw(b);
		window.draw(c);
		window.draw(d);
		window.draw(Question);
		window.draw(question);
		window.display();
	}
	return 0;
}
string getYear() {
	string sql = "SELECT Yearlvl FROM PlayerTable WHERE GameID = '" + GameID + "';";
	cout << GameID << "are";
	const char* dbName = "Unique.db"; // Replace with your database name
	const char* query = sql.c_str(); // Replace with your query

	const char* charValue = getCharValueFromDB(dbName, query);

	if (charValue != nullptr) {
		std::cout << "Character value: " << charValue << std::endl;

		// Compare retrieved charValue with "First"
		string retVal(charValue);
		return retVal;
	}


	else {
		std::cerr << "Failed to retrieve character value from the database." << std::endl;
		cout << GameID << "ass\n";
	}
	return "";
}
int game2(RenderWindow& window, int difficulty) {
	RectangleShape Question(Vector2f(1250, 400));
	Question.setOrigin(Vector2f(Question.getSize().x / 2, Question.getSize().y / 2));

	Vector2f mouse;
	View view(sf::FloatRect(-150, 0, window.getSize().x * 2, window.getSize().y * 2));
	window.setView(view);
	Question.setPosition(Vector2f(view.getCenter().x, view.getCenter().x - 600));

	while (window.isOpen())
	{


		Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
		mouse = window.mapPixelToCoords(Mouse::getPosition(window));




		window.clear();
		window.draw(Question);
		window.display();
	}


	return 0;
}
void CICS(RenderWindow& window, Texture& platex) {
	// Create Floor objects
	Floor chatBox(nullptr, Vector2f(window.getSize().x * 2.5, (window.getSize().y / 4) * 2.5), Vector2f(0, 0));
	Texture texture, texhallway;
	texture.loadFromFile("Graphics/rooom.png");
	texhallway.loadFromFile("Graphics/hallway.png");
	Floor oneroom(&texture, Vector2f(3500.0f, 2000.0f), Vector2f(40.0f, -20.0f));
	Floor onehallway(&texhallway, Vector2f(4000.0f, 500.0f), Vector2f(30.0f, 540.0f));
	Floor flooronehallway(nullptr, Vector2f(3510.0f, 100.0f), Vector2f(30.0f, 650.0f));
	Floor wall(nullptr, Vector2f(4000.0f, 30.0f), Vector2f(30.0f, 320.0f));
	Floor walltwo(nullptr, Vector2f(30.0f, 490.0f), Vector2f(2004.0f, 535.0f));
	Floor wallthree(nullptr, Vector2f(4000.0f, 30.0f), Vector2f(0.0f, 775.0f));
	Floor wallfour(nullptr, Vector2f(30.0f, 490.0f), Vector2f(-1958.0f, 535.0f));
	wall.blacked();
	walltwo.blacked();
	wallthree.blacked();
	wallfour.blacked();
	Enter flooronestairsright(Vector2f(1904, 672));
	blocks block(nullptr, Vector2f(3510, 300), Vector2f(30, 370));
	//Enter flooronestairsleft(Vector2f(-1663, 546));
	Enter flooroneroomoneleft(Vector2f(-1499, 570));
	Enter flooroneroomoneright(Vector2f(-832, 570));
	Enter flooroneroomtwoleft(Vector2f(-313, 570));
	Enter flooroneroomtworight(Vector2f(363, 570));
	Enter flooroneroomthreeleft(Vector2f(886, 570));
	Enter flooroneroomthreeright(Vector2f(1551, 570));
	block.transpa();
	Player you(&platex, Vector2u(3, 5), 0.3f, 250.0f);
	Collider youCollider = you.GetCollider();
	you.setPos(Vector2f(1804, 672));

	string def = " ";
	string dialogue;
	int secondsPassed = 0;
	float accumulatedTime = 0.0f;
	float deltaTime = 0.0f;
	Clock clock;

	Font font;
	font.loadFromFile("arial.ttf");
	Text convo;
	//TextBox tb(window, font, Vector2f(window.getSize().x / 5, window.getSize().y / 5), Vector2f((window.getSize().x / 5), (window.getSize().y / 5)));
	convo.setFont(font);
	convo.setCharacterSize(75);
	convo.setFillColor(Color::Black);


	View view(Vector2f(desc_x / 2, desc_y / 2), Vector2f(view_heightx * 4.5, view_heighty * 4.5));
	view.setCenter(Vector2f(24, 330));
	
	window.setView(view);
	chatBox.setPos(Vector2f(view.getCenter().x, view.getCenter().y + chatBox.getSize().y * 2.5));
	convo.setPosition(Vector2f(view.getCenter().x - (chatBox.getSize().x / 2.15), view.getCenter().y + chatBox.getSize().y * 2.25));

	while (window.isOpen()) {
		// cout << "one\n";
		convo.setString(def);
		sf::Vector2i windowMousePosition = sf::Mouse::getPosition(window);

		deltaTime = clock.restart().asSeconds();
		accumulatedTime += deltaTime;
		Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == Event::Closed)
				window.close();
			else if (ev.type == Event::Resized)
				resizeview(window, view);
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}


		// Clear the window before drawing
		window.clear(Color::Black);
		you.Update(deltaTime);
		wall.GetCollider().isColliding(youCollider, 1.0f);
		walltwo.GetCollider().isColliding(youCollider, 1.0f);
		wallthree.GetCollider().isColliding(youCollider, 1.0f);
		wallfour.GetCollider().isColliding(youCollider, 1.0f);
		block.GetCollider().isColliding(youCollider, 1);
		oneroom.Draw(window);
		onehallway.Draw(window);
		wall.Draw(window);
		walltwo.Draw(window);
		wallthree.Draw(window);
		wallfour.Draw(window);
		flooronestairsright.Draw(window);
		//flooronestairsleft.Draw(window);
		flooroneroomoneleft.Draw(window);
		flooroneroomoneright.Draw(window);
		flooroneroomtwoleft.Draw(window);
		flooroneroomtworight.Draw(window);
		flooroneroomthreeleft.Draw(window);
		flooroneroomthreeright.Draw(window);
		flooronehallway.Draw(window);
		you.Draw(window);
		if (you.bounds().intersects(flooronestairsright.bounds())) {
			floortwo(window, view, platex);

			cout << "x = " << you.GetPosition().x << " y = " << you.GetPosition().y << endl;

		}
		else if (you.bounds().intersects(flooroneroomoneleft.bounds())) {
			dialogue = "You can't enter here, classes on going";
			convo.setString(dialogue);
			chatBox.Draw(window);
			window.draw(convo);
		}
		else if (you.bounds().intersects(flooroneroomoneright.bounds())) {
			dialogue = "You can't enter here, classes on going";
			convo.setString(dialogue);
			chatBox.Draw(window);
			window.draw(convo);
		}
		else if (you.bounds().intersects(flooroneroomtwoleft.bounds())) {
			dialogue = "You can't enter here, classes on going";
			convo.setString(dialogue);
			chatBox.Draw(window);
			window.draw(convo);
		}
		else if (you.bounds().intersects(flooroneroomtworight.bounds())) {
			dialogue = "You can't enter here, classes on going";
			convo.setString(dialogue);
			chatBox.Draw(window);
			window.draw(convo);
		}
		else if (you.bounds().intersects(flooroneroomthreeleft.bounds())) {
			dialogue = "You can't enter here, classes on going";
			convo.setString(dialogue);
			chatBox.Draw(window);
			window.draw(convo);
		}
		else if (you.bounds().intersects(flooroneroomthreeright.bounds())) {
			dialogue = "You can't enter here, classes on going";
			convo.setString(dialogue);
			chatBox.Draw(window);
			window.draw(convo);
		}
		block.Draw(window);
		window.display();


		// cout << "x = " << you.GetPosition().x << " y = " << you.GetPosition().y << endl; //this will detect the position of the player
	}
}





void Register(int count, string name, char program, string dream) {
	sqlite3* db;
	char* errMsg = nullptr;
	int rc = sqlite3_open("Unique.db", &db);
	if (rc != SQLITE_OK) {
		std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		return;
	}
	string countstr = to_string(count + 1);
	string progstr;
	switch (program)
	{
	case 'a':
	case 'A':
		progstr = "BS Computer Science";
		break;
	case 'b':
	case 'B':
		progstr = "BS Computer Engineering";
		break;
	case 'c':
	case 'C':
		progstr = "BS Architecture";
		break;
	default:
		break;
	}
	string sqlQuery = "INSERT INTO GameTable (GameID) VALUES('GM0" + countstr + "');" + "INSERT INTO PlayerTable Values(" + "'PL0" + countstr + "'," + "'GM0" + countstr + "'," + " '" + name + "'," + "'" + progstr + "'," + "'First'," + "'" + dream + "',2110.00,2050.00);";
	GameID = "GM0" + countstr;
	rc = sqlite3_exec(db, sqlQuery.c_str(), nullptr, nullptr, &errMsg);

	if (rc != SQLITE_OK) {
		std::cerr << "SQL error: " << errMsg << std::endl;
		sqlite3_free(errMsg);
	}
	else {
		std::cout << "Data inserted successfully." << std::endl;
	}

	sqlite3_close(db);

}
void apply(RenderWindow& window) {

	sqlite3* db;
	int count = 0;
	char* errMsg = nullptr;

	int rc = sqlite3_open("Unique.db", &db); // Open your SQLite database

	if (rc != SQLITE_OK) {
		std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		return;
	}
	string sqlQuery = "SELECT COUNT(*) FROM GameTable;";

	rc = sqlite3_exec(db, sqlQuery.c_str(), idCallback, &count, &errMsg);

	if (rc != SQLITE_OK) {
		std::cerr << "SQL error: " << errMsg << std::endl;
		sqlite3_free(errMsg);
	}
	string name, dream;
	char prog;
	//add something the will determine this ff:: int count, string name, char program,string dream
	sqlite3_close(db); // Close the database connection

	Plant Guard(nullptr, Vector2f(window.getSize().x / 4, window.getSize().y * 0.75), Vector2f(window.getSize().x * 0.75, window.getSize().y * 0.5));
	Plant chatBox(nullptr, Vector2f(window.getSize().x, (window.getSize().y / 3)), Vector2f(window.getSize().x / 2, window.getSize().y * 0.80));
	chatBox.Transparent();
	View view(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
	window.setView(view);

	int secondsPassed = 0;//timer
	float accumulatedTime = 0.0f;
	//defining tick
	float deltaTime = 0.0f;
	Clock clock;

	Font font;
	font.loadFromFile("arial.ttf");
	Text convo;
	TextBox tb(window, font, Vector2f(window.getSize().x / 5, window.getSize().y / 5), Vector2f((window.getSize().x / 5), (window.getSize().y / 5)));
	convo.setFont(font);
	convo.setCharacterSize(20);
	convo.setFillColor(Color::Black);
	convo.setPosition(Vector2f(window.getSize().x / 11, window.getSize().y * 0.80));
	string x;
	int question = 0;
	while (window.isOpen()) {
		Event ev;// event for the window... 
		deltaTime = clock.restart().asSeconds();
		accumulatedTime += deltaTime;
		if (question == 0) {
			x = "Enter Your Name...";
		}
		else if (question == 1) {
			x = "choose you program:\n a.BS Computer Science\t\tb. BS Computer Engineering\n\tc. BS Architecture";
		}
		else if (question == 2) {
			x = "Enter Your Dream...";
		}
		else if (question == 3) {
			x = "Your Data is being registere please wait...\n click return to continue";
		}
		else
			return;
		if (accumulatedTime >= 1.0f) {
			secondsPassed += static_cast<int>(std::floor(accumulatedTime));
			accumulatedTime = fmod(accumulatedTime, 1.0f); // Store remaining time

		}
		//cout << secondsPassed << endl;
		while (window.pollEvent(ev)) {
			switch (ev.type) {
			case Event::Closed:
				window.close();
				break;
			}
			tb.handleInput(ev);
		}
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			if (question == 0) {
				name = tb.getInput();
				question++;
				tb.reset();
				sleep(seconds(1));
			}
			else if (question == 1) {
				prog = tb.getInput()[1];
				question++;
				tb.reset();
				sleep(seconds(1));
			}
			else if (question == 2) {
				dream = tb.getInput();
				question++;
				tb.reset();
				sleep(seconds(1));
			}
			else if (question == 3) {
				Register(count, name, prog, dream);
				question++;
				sleep(seconds(1));
				return;

			}
		}
		if (secondsPassed > 30) {
			return;
		}
		convo.setString(x);
		window.display();
		window.clear(Color::Blue);
		Guard.Draw(window);
		chatBox.Draw(window);
		tb.Draw();
		window.draw(convo);

	}
}////get info get info

void SSC(RenderWindow& window) {
	apply(window);
}

void CEAFA(RenderWindow& window,Texture & platex) {
		// Create Floor objects
		Floor chatBox(nullptr, Vector2f(window.getSize().x * 2.5, (window.getSize().y / 4) * 2.5), Vector2f(0, 0));
		Texture texture, textrophy1, texttrophy2;
		texture.loadFromFile("Graphics/roooom.png");
		textrophy1.loadFromFile("trophy1.png");
		texttrophy2.loadFromFile("trophy2.png");
		Floor onecanteen(&texture, Vector2f(5400.0f, 2500.0f), Vector2f(-90.0f, -5.0f));
		Floor hallway(nullptr, Vector2f(5000.0f, 200.0f), Vector2f(60.0f, 200.0f));
		blocks trophy(&textrophy1, Vector2f(206.0f, 532.0f), Vector2f(-594, 678.0f));
		blocks trophytwo(&texttrophy2, Vector2f(206.0f, 532.0f), Vector2f(636.0f, 678.0f));
		blocks stairs(nullptr, Vector2f(600.0f, 600.0f), Vector2f(71.0f, 1250.0f));
		blocks wall1(nullptr, Vector2f(50, 630), Vector2f(1385, -198));
		blocks wall2(nullptr, Vector2f(50, 630), Vector2f(550, -198));
		blocks wall3(nullptr, Vector2f(50, 630), Vector2f(43, -198));
		blocks wall4(nullptr, Vector2f(50, 630), Vector2f(-521, -198));
		blocks wall5(nullptr, Vector2f(50, 630), Vector2f(-1343, -198));
		blocks wall6(nullptr, Vector2f(5400, 30), Vector2f(-90, -480));
		blocks wall7(nullptr, Vector2f(1750, 30), Vector2f(-1383, 383));
		blocks wall8(nullptr, Vector2f(1750, 30), Vector2f(1396, 383));
		blocks wall9(nullptr, Vector2f(979, 30), Vector2f(28, 872));
		blocks wall(nullptr, Vector2f(4500.0f, 30.0f), Vector2f(50.0f, 140.0f));
		blocks leftwall(nullptr, Vector2f(50.0f, 997.0f), Vector2f(-2120.0f, -54));
		blocks rightwall(nullptr, Vector2f(50.0f, 997.0f), Vector2f(2155.0f, -54.0f));
		Enter sideright(Vector2f(-1029, 239));
		Enter leftside(Vector2f(-1958, 239));
		Enter flooronestairs(Vector2f(99, 900));
		Enter canteen(Vector2f(-35, 205));
		trophy.transpa();
		trophytwo.transpa();
		stairs.transpa();
		wall1.transpa(); wall2.transpa(); wall3.transpa(); wall4.transpa(); wall5.transpa(); wall6.transpa(); wall7.transpa(); wall8.transpa(); wall9.transpa();leftwall.transpa(); rightwall.transpa();
		Player you(&platex, Vector2u(3, 5), 0.3f, 250.0f);
		you.setPos(Vector2f(99, 900));
		Collider youCollider = you.GetCollider();

		string def = " ";
		string dialogue;
		int secondsPassed = 0;
		float accumulatedTime = 0.0f;
		float deltaTime = 0.0f;
		Clock clock;

		Font font;
		font.loadFromFile("arial.ttf");
		Text convo;
		TextBox tb(window, font, Vector2f(window.getSize().x / 5, window.getSize().y / 5), Vector2f((window.getSize().x / 5), (window.getSize().y / 5)));
		convo.setFont(font);
		convo.setCharacterSize(75);
		convo.setFillColor(Color::Black);

		View view(Vector2f(desc_x/2, desc_y/2), Vector2f(view_heightx * 4.5, view_heighty * 4.5));			
		view.setCenter(Vector2f(24, 330));
		window.setView(view);
		chatBox.setPos(Vector2f(view.getCenter().x, view.getCenter().y + chatBox.getSize().y * 2.5));
		convo.setPosition(Vector2f(view.getCenter().x - (chatBox.getSize().x / 2.15), view.getCenter().y + chatBox.getSize().y * 2.25));

		while (window.isOpen()) {
			convo.setString(def);
			sf::Vector2i windowMousePosition = sf::Mouse::getPosition(window);

			deltaTime = clock.restart().asSeconds();
			accumulatedTime += deltaTime;
			Event ev;
			while (window.pollEvent(ev)) {
				if (ev.type == Event::Closed)
					window.close();
				else if (ev.type == Event::Resized)
					resizeview(window, view);
			}

			// Clear the window before drawing
			window.clear(Color::Green);
			you.Update(deltaTime);
			trophy.GetCollider().isColliding(youCollider, 1.0f);
			wall1.GetCollider().isColliding(youCollider, 1.0f);
			wall2.GetCollider().isColliding(youCollider, 1.0f);
			wall3.GetCollider().isColliding(youCollider, 1.0f);
			wall4.GetCollider().isColliding(youCollider, 1.0f);
			wall5.GetCollider().isColliding(youCollider, 1.0f);
			wall6.GetCollider().isColliding(youCollider, 1.0f);
			wall7.GetCollider().isColliding(youCollider, 1.0f);
			wall8.GetCollider().isColliding(youCollider, 1.0f);
		

			stairs.Draw(window);
			wall.Draw(window);
			canteen.Draw(window);
			hallway.Draw(window);
			onecanteen.Draw(window);
			wall1.Draw(window);
			wall2.Draw(window);
			wall3.Draw(window);
			wall4.Draw(window);
			wall5.Draw(window);
			wall6.Draw(window);
			wall7.Draw(window);
			wall8.Draw(window);
			wall9.Draw(window);
		
			trophy.Draw(window);
			trophytwo.Draw(window);
			leftwall.Draw(window);
			rightwall.Draw(window);
			leftside.Draw(window);
			sideright.Draw(window);
			flooronestairs.Draw(window);


			you.Draw(window);

			if (you.bounds().intersects(flooronestairs.bounds())) {
				dialogue = "You can't enter here, under construction!";
				convo.setString(dialogue);
				chatBox.Draw(window);
				window.draw(convo);
			}
			else if (you.bounds().intersects(leftside.bounds())) {
				dialogue = "You can't enter here";
				convo.setString(dialogue);
				chatBox.Draw(window);
				window.draw(convo);
			}
			else if (you.bounds().intersects(sideright.bounds())) {
				dialogue = "You can't enter here";
				convo.setString(dialogue);
				chatBox.Draw(window);
				window.draw(convo);
			}

			window.display();

			cout << "x = " << you.GetPosition().x << " y = " << you.GetPosition().y << endl; //this will detect the position of the player

		}
	}

struct CountData {
	int countValue;
};

static int callbackcount(void* data, int argc, char** argv, char** azColName) {
	CountData* countData = static_cast<CountData*>(data);
	if (argc > 0) {
		countData->countValue = std::stoi(argv[0]); // Assuming count value is in the first column
	}
	return 0;
}

static int getCount(const char* dir) {
	sqlite3* DB;
	int exit = sqlite3_open(dir, &DB);
	if (exit) {
		std::cerr << "Error opening database: " << sqlite3_errmsg(DB) << std::endl;
		sqlite3_close(DB);
		return -1; // Return an error code
	}

	const char* sql = "SELECT COUNT(GameID) AS count FROM GameTable;"; // Change YourTable to your table name

	CountData countData;
	countData.countValue = 0; // Initialize count value

	exit = sqlite3_exec(DB, sql, callbackcount, &countData, nullptr);
	if (exit != SQLITE_OK) {
		std::cerr << "Error executing SQL: " << sqlite3_errmsg(DB) << std::endl;
		sqlite3_close(DB);
		return -1; // Return an error code
	}

	sqlite3_close(DB);
	return countData.countValue; // Return the count value
}


string LoadButtonText(const char* dir, int step) {
	sqlite3* DB;
	sqlite3_stmt* stmt;

	int exit = sqlite3_open(dir, &DB);
	if (exit) {
		std::cerr << "Error opening database: " << sqlite3_errmsg(DB) << std::endl;
		sqlite3_close(DB);
		return ""; // Return empty string on error
	}

	exit = sqlite3_prepare_v2(DB, "SELECT GameID FROM GameTable", -1, &stmt, 0);
	if (exit != SQLITE_OK) {
		std::cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << std::endl;
		sqlite3_finalize(stmt);
		sqlite3_close(DB);
		return ""; // Return empty string on error
	}

	// Step through the rows (assuming you want to retrieve 'step' rows)
	for (int i = 0; i < step; i++) {
		int res = sqlite3_step(stmt);
		if (res != SQLITE_ROW) {
			std::cerr << "Error stepping through rows: " << sqlite3_errmsg(DB) << std::endl;
			sqlite3_finalize(stmt);
			sqlite3_close(DB);
			return ""; // Return empty string on error
		}
	}

	// Retrieve GameID from the current row
	const unsigned char* GameID = sqlite3_column_text(stmt, 0);
	std::string GameName(reinterpret_cast<const char*>(GameID));

	// Finalize the statement and close the database
	sqlite3_finalize(stmt);
	sqlite3_close(DB);

	return GameName;
}

void LoadState(RenderWindow& window,Vector2f& mouse, const char* dir) {
	int count = getCount(dir);
	Vector2f buttonloc = Vector2f(window.getSize().x / 2, (window.getSize().y / 4));
	vector<Button> loadGame;
	vector<Text> GameName;
	Font font;
	font.loadFromFile("pix.ttf");
	
	for (int i = 1; i <= count; i++) {
		Button btn("Graphics/button.png", Vector2f(200,50), Vector2f(buttonloc.x, buttonloc.y + ((i - 1) * 75)));
		Text text;
		text.setFont(font);
		text.setString(LoadButtonText(dir, i));
		text.setFillColor(Color::White);
		text.setCharacterSize(19);
		FloatRect textBounds = text.getLocalBounds();
		text.setOrigin(textBounds.left + textBounds.width / 2.0f,
						textBounds.top + textBounds.height / 2.0f);
		text.setPosition(Vector2f(btn.getpos().x , btn.getpos().y-5));

		GameName.push_back(text);
		loadGame.push_back(btn);
	}
	cout << count;
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
		mouse = window.mapPixelToCoords(Mouse::getPosition(window));

		RectangleShape back;
		back.setPosition(Vector2f(30, 30));
		back.setSize(Vector2f(200, 50));

		if (back.getGlobalBounds().contains(mouse)) {
			back.setFillColor(Color::Red);
			if (Mouse::isButtonPressed(Mouse::Left)){
				return;
			}
			
		}
		else {
			back.setFillColor(Color::Blue);
		}
		
		int i = 0;
		
		for (Button& butt : loadGame) {
			i++;
			if (butt.bounds().contains(mouse))
				{
					butt.Hover();
					if (Mouse::isButtonPressed(Mouse::Left)) {
						butt.Click();
						GameID = GameName[i-1].getString();
						
					}
				}
			else {
				butt.Def();
			}


			butt.Draw(window);
		}
		for (Text& txt : GameName) {
			window.draw(txt);
		}
		cout << GameID;
		window.draw(back);
		window.display();
		window.clear();
	}
}

void SettingState(RenderWindow& window, Vector2f& mouse,int& sfx, int& vol) {

	RectangleShape back,speaker,sfxvolume,soundfx,svolume;
	CircleShape sfxlvl, slvl;
	
	back.setPosition(Vector2f(30, 30));
	back.setSize(Vector2f(200, 50));

	sfxvolume.setSize(Vector2f(250,20));
	svolume.setSize(Vector2f(250, 20));
	speaker.setSize(Vector2f(50, 50));
	soundfx.setSize(Vector2f(50, 50));

	speaker.setOrigin(Vector2f(speaker.getSize().x / 2, speaker.getSize().y / 2));
	sfxvolume.setOrigin(Vector2f(sfxvolume.getSize().x / 2, speaker.getSize().y / 2));
	soundfx.setOrigin(Vector2f(soundfx.getSize().x / 2, speaker.getSize().y / 2));
	svolume.setOrigin(Vector2f(svolume.getSize().x / 2, speaker.getSize().y / 2));

	speaker.setPosition(Vector2f((window.getSize().x / 2) - svolume.getSize().x *0.5, (window.getSize().y / 2) - 50));
	soundfx.setPosition(Vector2f((window.getSize().x / 2) - sfxvolume.getSize().x *0.5, (window.getSize().y / 2) + 50));
	sfxvolume.setPosition(Vector2f(speaker.getPosition().x + svolume.getSize().x -30, speaker.getPosition().y + sfxvolume.getSize().y));
	svolume.setPosition(Vector2f(soundfx.getPosition().x + sfxvolume.getSize().x -30, soundfx.getPosition().y + svolume.getSize().y));

	sfxlvl.setRadius(15);
	slvl.setRadius(15);
	sfxlvl.setPosition(Vector2f(sfxvolume.getPosition().x, sfxvolume.getPosition().y - sfxlvl.getRadius() * 2));
	slvl.setPosition(Vector2f(svolume.getPosition().x, svolume.getPosition().y - slvl.getRadius()*2));
	

	while (window.isOpen())
	{
		mouse = window.mapPixelToCoords(Mouse::getPosition(window));

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		if(sfxlvl.getGlobalBounds().contains(mouse) && sfxvolume.getGlobalBounds().contains(mouse)){
			if (Mouse::isButtonPressed(Mouse::Left)) {
				sfxlvl.setPosition(Vector2f(mouse.x - sfxlvl.getRadius(), sfxlvl.getPosition().y));
				sfx = static_cast<int>((sfxlvl.getPosition().x - (sfxvolume.getPosition().x - (sfxvolume.getSize().x / 2) - sfxlvl.getRadius())) / 2.5);
				cout << "sfx :: " << sfx << endl;
			}
			
		
		}

		if (slvl.getGlobalBounds().contains(mouse) && svolume.getGlobalBounds().contains(mouse)) {
			if (Mouse::isButtonPressed(Mouse::Left)) {
				slvl.setPosition(Vector2f(mouse.x - slvl.getRadius(), slvl.getPosition().y));
				vol = static_cast<int>((slvl.getPosition().x - (svolume.getPosition().x - (svolume.getSize().x/2) - slvl.getRadius()))/2.5);
				cout << "sound :: " << vol << endl;
			
			}
				
			
		}

		if (back.getGlobalBounds().contains(mouse)) {
			back.setFillColor(Color::Red);
			if (Mouse::isButtonPressed(Mouse::Left)) {
				return;
			}

		}
		else {
			back.setFillColor(Color::Blue);
		}


		window.draw(back);
		window.draw(soundfx);
		window.draw(svolume);
		window.draw(sfxvolume);
		window.draw(speaker);
		window.draw(sfxlvl);
		window.draw(slvl);




		window.display();
		window.clear();
	}
}
void floortwo(RenderWindow& window, View& view, Texture& platex) {
	// Create Floor objects
	Floor chatBox(nullptr, Vector2f(window.getSize().x * 2.5, (window.getSize().y / 4) * 2.5), Vector2f(0, 0));
	Texture texture, texhallway;
	texture.loadFromFile("Graphics/rooom.png");
	texhallway.loadFromFile("Graphics/hallway.png");
	Floor tworoom(&texture, Vector2f(3500.0f, 2000.0f), Vector2f(40.0f, -20.0f));
	Floor twohallway(&texhallway, Vector2f(4000.0f, 500.0f), Vector2f(30.0f, 540.0f));
	Floor floortwohallway(nullptr, Vector2f(3510.0f, 100.0f), Vector2f(30.0f, 650.0f));
	Floor wall(nullptr, Vector2f(4000.0f, 30.0f), Vector2f(30.0f, 320.0f));
	Floor walltwo(nullptr, Vector2f(30.0f, 490.0f), Vector2f(2004.0f, 535.0f));
	Floor wallthree(nullptr, Vector2f(4000.0f, 30.0f), Vector2f(0.0f, 775.0f));
	Floor wallfour(nullptr, Vector2f(30.0f, 490.0f), Vector2f(-1958.0f, 535.0f));
	wall.blacked();
	walltwo.blacked();
	wallthree.blacked();
	wallfour.blacked();
	Enter floortwostairsright(Vector2f(1904, 672));
	blocks block(nullptr, Vector2f(3510, 300), Vector2f(30, 370));
	Enter floortwostairsleft(Vector2f(-1846, 672));
	Enter floortworoomoneleft(Vector2f(1551, 570));
	Enter floortworoomoneright(Vector2f(875, 570));
	Enter floortworoomtwoleft(Vector2f(340, 570));
	Enter floortworoomtworight(Vector2f(-315, 570));
	Enter floortworoomthreeleft(Vector2f(-844, 570));
	Enter floortworoomthreeright(Vector2f(-1517, 570));
	block.transpa();
	Player you(&platex, Vector2u(3, 5), 0.3f, 250.0f);
	you.setPos(Vector2f(1804, 672));
	Collider youCollider = you.GetCollider();

	string def = " ";
	string dialogue;
	int secondsPassed = 0;
	float accumulatedTime = 0.0f;
	float deltaTime = 0.0f;
	Clock clock;

	Font font;
	font.loadFromFile("arial.ttf");
	Text convo;
	//TextBox tb(window, font, Vector2f(window.getSize().x / 5, window.getSize().y / 5), Vector2f((window.getSize().x / 5), (window.getSize().y / 5)));
	convo.setFont(font);
	convo.setCharacterSize(75);
	convo.setFillColor(Color::Black);


	
	chatBox.setPos(Vector2f(view.getCenter().x, view.getCenter().y + chatBox.getSize().y * 2.5));
	convo.setPosition(Vector2f(view.getCenter().x - (chatBox.getSize().x / 2.15), view.getCenter().y + chatBox.getSize().y * 2.25));

	while (window.isOpen()) {
		cout << "two\n";
		convo.setString(def);
		sf::Vector2i windowMousePosition = sf::Mouse::getPosition(window);

		deltaTime = clock.restart().asSeconds();
		accumulatedTime += deltaTime;
		Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == Event::Closed)
				window.close();
			else if (ev.type == Event::Resized)
				resizeview(window, view);
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}


		// Clear the window before drawing
		window.clear(Color::Black);
		you.Update(deltaTime);
		wall.GetCollider().isColliding(youCollider, 1.0f);
		walltwo.GetCollider().isColliding(youCollider, 1.0f);
		wallthree.GetCollider().isColliding(youCollider, 1.0f);
		wallfour.GetCollider().isColliding(youCollider, 1.0f);
		block.GetCollider().isColliding(youCollider, 1);
		tworoom.Draw(window);
		twohallway.Draw(window);
		wall.Draw(window);
		walltwo.Draw(window);
		wallthree.Draw(window);
		wallfour.Draw(window);
		floortwostairsright.Draw(window);
		floortwostairsleft.Draw(window);
		floortworoomoneleft.Draw(window);
		floortworoomoneright.Draw(window);
		floortworoomtwoleft.Draw(window);
		floortworoomtworight.Draw(window);
		floortworoomthreeleft.Draw(window);
		floortworoomthreeright.Draw(window);
		floortwohallway.Draw(window);
		you.Draw(window);
		if (you.bounds().intersects(floortwostairsright.bounds())) {
			floorthreeroomone(window, view, platex);

			cout << "x = " << you.GetPosition().x << " y = " << you.GetPosition().y << endl;
		}
		else if (you.bounds().intersects(floortwostairsleft.bounds())) {
			CICS(window, platex);
		}
		else if (you.bounds().intersects(floortworoomoneleft.bounds())) {
			dialogue = "You can't enter here, classes on going";
			convo.setString(dialogue);
			chatBox.Draw(window);
			window.draw(convo);
		}
		else if (you.bounds().intersects(floortworoomoneright.bounds())) {
			dialogue = "You can't enter here, classes on going";
			convo.setString(dialogue);
			chatBox.Draw(window);
			window.draw(convo);
		}
		else if (you.bounds().intersects(floortworoomtwoleft.bounds())) {
			dialogue = "You can't enter here, classes on going";
			convo.setString(dialogue);
			chatBox.Draw(window);
			window.draw(convo);
		}
		else if (you.bounds().intersects(floortworoomtworight.bounds())) {
			dialogue = "You can't enter here, classes on going";
			convo.setString(dialogue);
			chatBox.Draw(window);
			window.draw(convo);
		}
		else if (you.bounds().intersects(floortworoomthreeleft.bounds())) {
			dialogue = "You can't enter here, classes on going";
			convo.setString(dialogue);
			chatBox.Draw(window);
			window.draw(convo);
		}
		else if (you.bounds().intersects(floortworoomthreeright.bounds())) {
			dialogue = "You can't enter here, classes on going";
			convo.setString(dialogue);
			chatBox.Draw(window);
			window.draw(convo);
		}
		block.Draw(window);
		window.display();


		cout << "x = " << you.GetPosition().x << " y = " << you.GetPosition().y << endl; //this will detect the position of the player
	}

}

void floorthreeroomone(RenderWindow& window, View& view, Texture& platex) {
	// Create Floor objects
	Floor chatBox(nullptr, Vector2f(window.getSize().x * 2.5, (window.getSize().y / 4) * 2.5), Vector2f(0, 0));
	Texture texture, texhallway;
	texture.loadFromFile("Graphics/thirdfloor.png");
	texhallway.loadFromFile("Graphics/hallway2.png");

	Floor roomone(&texture, Vector2f(3500.0f, 1000.0f), Vector2f(17.0f, -5.0f));
	Floor hallway(&texhallway, Vector2f(4100.0f, 500.0f), Vector2f(30.0f, 950.0f));
	Floor floor(nullptr, Vector2f(4100.0f, 200.0f), Vector2f(30.0f, 850.0f));
	blocks tableone(nullptr, Vector2f(1300.0f, 500.0f), Vector2f(-781, 131));
	blocks tabletwo(nullptr, Vector2f(1300.0f, 500.0f), Vector2f(885, 131));
	blocks walldown(nullptr, Vector2f(10000.0f, 30.0f), Vector2f(60.0f, 503.0f));
	blocks wallright(nullptr, Vector2f(30.0f, 1500.0f), Vector2f(1750.0f, 18.0f));
	blocks wallup(nullptr, Vector2f(10000.0f, 30.0f), Vector2f(100.0f, -595.0f));
	blocks wallleft(nullptr, Vector2f(30.0f, 1000.0f), Vector2f(-1760.0f, -64.0f));
	blocks wall(nullptr, Vector2f(10000.0f, 200.0f), Vector2f(-64.0f, -169.0f));

	Enter leftdoor(Vector2f(-1509.0f, 438.0f));
	Enter rightdoor(Vector2f(1585.0f, 1000.0f));
	Enter leftstairs(Vector2f(-1874.0f, 1065.0f));
	Enter rightstairs(Vector2f(1949, 1065));
	walldown.transpa();
	tableone.transpa();
	tabletwo.transpa();
	wallright.transpa();
	wallleft.transpa();
	wallup.transpa();
	wall.transpa();


	Player you(&platex, Vector2u(3, 5), 0.3f, 250.0f);
	you.setPos(Vector2f(1800, 1000));
	Collider youCollider = you.GetCollider();

	string def = " ";
	string dialogue;
	int secondsPassed = 0;
	float accumulatedTime = 0.0f;
	float deltaTime = 0.0f;
	Clock clock;

	Font font;
	font.loadFromFile("arial.ttf");
	Text convo;
	convo.setFont(font);
	convo.setCharacterSize(75);
	convo.setFillColor(Color::Black);


	view.setCenter(Vector2f(24, 330));
	window.setView(view);
	chatBox.setPos(Vector2f(view.getCenter().x, view.getCenter().y + chatBox.getSize().y * 2.5));
	convo.setPosition(Vector2f(view.getCenter().x - (chatBox.getSize().x / 2.15), view.getCenter().y + chatBox.getSize().y * 2.25));

	while (window.isOpen()) {
		convo.setString(def);
		sf::Vector2i windowMousePosition = sf::Mouse::getPosition(window);

		deltaTime = clock.restart().asSeconds();
		accumulatedTime += deltaTime;
		Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == Event::Closed)
				window.close();
			else if (ev.type == Event::Resized)
				resizeview(window, view);
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}


		// Clear the window before drawing

		you.Update(deltaTime);
		floor.GetCollider().isColliding(youCollider, 1.0f);
		tableone.GetCollider().isColliding(youCollider, 1.0f);
		tabletwo.GetCollider().isColliding(youCollider, 1.0f);
		wall.GetCollider().isColliding(youCollider, 1.0f);
		wallup.GetCollider().isColliding(youCollider, 1.0f);
		wallleft.GetCollider().isColliding(youCollider, 1.0f);
		wallright.GetCollider().isColliding(youCollider, 1.0f);
		walldown.GetCollider().isColliding(youCollider, 1.0f);
		if (you.bounds().intersects(rightdoor.bounds())) {
			you.setPos(Vector2f(1572, 370));
		}
		else if (you.bounds().intersects(leftdoor.bounds())) {
			you.setPos(Vector2f(-1547, 1000));
		}

		window.clear(Color::Black);
		roomone.Draw(window);
		tableone.Draw(window);
		tabletwo.Draw(window);
		wall.Draw(window);
		wallup.Draw(window);
		wallright.Draw(window);
		wallleft.Draw(window);
		walldown.Draw(window);
		floor.Draw(window);
		hallway.Draw(window);
		leftdoor.Draw(window);
		rightdoor.Draw(window);
		leftstairs.Draw(window);
		rightstairs.Draw(window);
		you.Draw(window);

		if (you.bounds().intersects(leftstairs.bounds())) {
			floortwo(window, view, platex);
		}
		else if (you.bounds().intersects(rightstairs.bounds())) {
			dialogue = "You can't enter here";
			convo.setString(dialogue);
			chatBox.Draw(window);
			window.draw(convo);
		}
		window.display();


		cout << "x = " << you.GetPosition().x << " y = " << you.GetPosition().y << endl; //this will detect the position of the player
	}

}
void newGame(RenderWindow& window) {
	sqlite3* db;
	sqlite3_open("Game.db", &db);
	// getting desktop size
	float w = VideoMode::getDesktopMode().width;
	float h = VideoMode::getDesktopMode().height;

	sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
	Texture kal;
	kal.loadFromFile("kalsada.png");

	// Output the screen size
	Font font;
	font.loadFromFile("arial.ttf");
	Text convo;
	convo.setFont(font);
	convo.setCharacterSize(60);
	convo.setFillColor(Color::Black);

	RectangleShape road(Vector2f(4734,898));
	road.setPosition(Vector2f(-198, 2985));
	//creating pov for the user
	View view(Vector2f(w / 4, h / 4), Vector2f(view_heightx * 3, view_heighty * 3));
	road.setTexture(&kal);
	//accessing image 
	Texture platex;
	platex.loadFromFile("char.png");

	Texture texceafa, texcics, texaces, texfdc, texsteerhub, texssc, texcit, texstudyarea;
	texceafa.loadFromFile("Graphics/ceafa.png");
	texcics.loadFromFile("Graphics/cics.png");
	texaces.loadFromFile("Graphics/aces.png");
	texfdc.loadFromFile("Graphics/fdc.png");
	texsteerhub.loadFromFile("Graphics/steerhub.png");
	texssc.loadFromFile("Graphics/registrar.png");
	texcit.loadFromFile("Graphics/cit.png");
	texstudyarea.loadFromFile("Graphics/studyarea.png");
	RectangleShape bg(Vector2f(5188, 4151));
	bg.setPosition(-227, -1203);
	Texture bgtex,pader;
	bgtex.loadFromFile("Graphics/bgfinalist.png");
	bg.setTexture(&bgtex);
	Building chatBox(nullptr, Vector2f(window.getSize().x * 2.5, (window.getSize().y / 4) * 2.5), Vector2f(0, 0));
	Building ceafa(&texceafa, Vector2f(916, 1250.0f), Vector2f(359.0f, 402.0f));
	Building cics(&texcics, Vector2f(462, 1251), Vector2f(3232.0f, -397.0f));
	Building aces(&texaces, Vector2f(1443.0f, 704), Vector2f(1655.0f, -700.0f));
	Building fdc(&texfdc, Vector2f(1271.0f, 661.0f), Vector2f(3134.0f, 570.0f));
	Building steerhub(&texsteerhub, Vector2f(1122.0f, 918.0f), Vector2f(4336.0f, 852.0f));
	Building ssc(&texssc, Vector2f(543.0f, 611.0f), Vector2f(2752.0f, 2000.0f));
	Building cit(&texcit, Vector2f(462.0f, 1247.0f), Vector2f(4462.0f, 2241.0f));
	Building studyarea(&texstudyarea, Vector2f(571.0f, 480.0f), Vector2f(2708.0f, -161.0f));
	Building gatethree(nullptr, Vector2f(700.0f, 150.0f), Vector2f(1100.0f, 2800.0f));
	Building gatetwo(nullptr, Vector2f(500.0f, 350.0f), Vector2f(3433.0f, 2755.0f));
	Building gatefour(nullptr, Vector2f(5188.0f, 30.0f), Vector2f(2524.0f, 2924.0f));
	Building wallgateone(nullptr, Vector2f(150.0f, 3970.0f), Vector2f(4861.0f, 980.0f));
	Building walltwo(nullptr, Vector2f(5145.0f, 150.0f), Vector2f(2370.0f, -1080.0f));
	Building wallthree(nullptr, Vector2f(150.0f, 2000.0f), Vector2f(-125.0f, -100.0f));
	Building wallfour(nullptr, Vector2f(910.0f, 150.0f), Vector2f(370.0f, 825.0f));
	Building wallfive(nullptr, Vector2f(150.0f, 2000.0f), Vector2f(750.0f, 1900.0f));
	Building gateone(nullptr, Vector2f(400.0f, 150.0f), Vector2f(4630.0f, 2900.0f));
	Enter gate(Vector2f(3433, 3045));
	Enter sscGate(Vector2f(3048, 2000));
	Enter ceafa1Gate(Vector2f(842, 133));
	Enter ceafa2Gate(Vector2f(842, 662));
	Enter gymGate(Vector2f(2473, 637));
	Enter cics1Gate(Vector2f(2976, 147));
	//Enter cics2Gate(Vector2f(1825,-600));
	Enter acesGate(Vector2f(2026, -298));
	Enter citGate(Vector2f(4206, 2193));
	Enter steer_hubGate(Vector2f(4371, 1361));
	//creating the player (texture, image count, switch time, speed
	string dialogue;
	Player you(&platex, Vector2u(3, 5), 0.3f, 250.0f);
	Collider youCollide = you.GetCollider();
	int secondsPassed = 0;//timer
	float accumulatedTime = 0.0f;
	//defining tick
	float deltaTime = 0.0f;
	Clock clock;

	//will show the desktop screen size
	//cout << "Screen Size :: x = " << w << " y = " << h << endl;


	while (window.isOpen()) { // game loop'
		sf::Vector2i windowMousePosition = sf::Mouse::getPosition(window);
		chatBox.setPos(Vector2f(you.GetPosition().x, you.GetPosition().y + (window.getSize().y * 0.9)));
		convo.setPosition(Vector2f(you.GetPosition().x - (window.getSize().x * 1.2), you.GetPosition().y + (window.getSize().y * 0.6)));
		// Output the mouse position
	//	std::cout << "Window Mouse X: " << windowMousePosition.x << ", Window Mouse Y: " << windowMousePosition.y << std::endl;
		deltaTime = clock.restart().asSeconds();
		accumulatedTime += deltaTime;

		if (accumulatedTime >= 1.0f) {
			secondsPassed += static_cast<int>(std::floor(accumulatedTime));
			accumulatedTime = fmod(accumulatedTime, 1.0f); // Store remaining time

		}
		//cout << secondsPassed << endl;
		Event ev;// event for the window... 
		while (window.pollEvent(ev)) { //eto yung nagdedetect kung clinick ba yung "x" sa window or nag resize, zoom , etc
			switch (ev.type) {
			case Event::Closed:
				posUpdate(you.GetPosition().x,you.GetPosition().y);
				window.close();
				break;
			case Event::Resized:
				resizeview(window, view);
				break;
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape)) { // press esc button to exit,
			posUpdate(you.GetPosition().x, you.GetPosition().y);
			window.close(); //  ginawa ko lang to nung tinatry ko sa fullscren kasi wala akong access sa x button

		}
		if (Keyboard::isKeyPressed(Keyboard::LShift)) { // press esc button to exit,
			you.setSpeed(2000);

		}
		else {
			you.setSpeed(250);
		}
		cout << "x = " << you.GetPosition().x << " y = " << you.GetPosition().y << endl; //this will detect the position of the player



		you.Update(deltaTime); // using the tick
		cics.GetCollider().isColliding(youCollide, 1.0f);
		ceafa.GetCollider().isColliding(youCollide, 1.0f);
		aces.GetCollider().isColliding(youCollide, 1.0f);
		fdc.GetCollider().isColliding(youCollide, 1.0f);
		steerhub.GetCollider().isColliding(youCollide, 1.0f);
		ssc.GetCollider().isColliding(youCollide, 1.0f);
		cit.GetCollider().isColliding(youCollide, 1.0f);
		studyarea.GetCollider().isColliding(youCollide, 1.0f);
		gatefour.GetCollider().isColliding(youCollide, 1.0f);
		view.setCenter(you.GetPosition());//setting the pov

		// Clear the window before drawing
		window.clear(Color::Green); //declaring bg


		window.setView(view); // using the pov
		window.draw(road);
		window.draw(bg);

		cics.Draw(window);
		ceafa.Draw(window);
		aces.Draw(window);
		fdc.Draw(window);
		steerhub.Draw(window);
		ssc.Draw(window);
		cit.Draw(window);
		studyarea.Draw(window);
		gatefour.Draw(window);

		gate.Draw(window);
		ceafa1Gate.Draw(window);
		ceafa2Gate.Draw(window);
		gymGate.Draw(window);
		cics1Gate.Draw(window);
		//cics2Gate.Draw(window);
		acesGate.Draw(window);
		citGate.Draw(window);
		sscGate.Draw(window);
		steer_hubGate.Draw(window);
		if (secondsPassed > 5) {
			gate.Draw(window);
		}

		if (secondsPassed < 10) {
			if (Keyboard::isKeyPressed(Keyboard::Space))//skip
				secondsPassed = 11;
			if (secondsPassed < 5)
				dialogue = "You can use 'W','A','S' and 'D' to move your character.";
			else
				dialogue = "Go in the circle to enter the university.";
			chatBox.Draw(window);
			window.draw(convo);
		}
		if (you.bounds().intersects(gate.bounds())) {
			InteractionGuard(window);
			you.setPos(Vector2f(3446, 2833));
		}
		else if (you.bounds().intersects(sscGate.bounds())) {
			SSC(window);
			you.setPos(Vector2f(2110, 2110));
		}
		else if (you.bounds().intersects(ceafa1Gate.bounds())) {
			CEAFA(window,platex);
		}
		else if (you.bounds().intersects(ceafa2Gate.bounds())) {
			CEAFA(window,platex);
		}
		else if (you.bounds().intersects(gymGate.bounds())) {
			dialogue = "You dont have any business in here";
			chatBox.Draw(window);
			window.draw(convo);
		}
		else if (you.bounds().intersects(cics1Gate.bounds())) {
			CICS(window,platex);
		}
		//else if (you.bounds().intersects(cics2Gate.bounds())) {
			//CICS(window);
		//}
		else if (you.bounds().intersects(acesGate.bounds())) {
			dialogue = "You dont have any business in here";
			chatBox.Draw(window);
			window.draw(convo);
		}
		else if (you.bounds().intersects(citGate.bounds())) {
			dialogue = "You dont have any business in here";
			chatBox.Draw(window);
			window.draw(convo);
		}
		else if (you.bounds().intersects(steer_hubGate.bounds())) {
			dialogue = "You dont have any business in here";
			chatBox.Draw(window);
			window.draw(convo);
		}
		convo.setString(dialogue);
		
		you.Draw(window);

		//drwing the player to the window

		// Display the window after drawing
		window.display();
	}

}

int main() {
	//database location
	const char* dir = "C:\\Users\\suman\\source\\repos\\Menu\\Menu\\Unique.db";


	Vector2f mouse;
	RenderWindow window(VideoMode(desc_x/2, desc_y/2), "BSU-rahan", Style::Default);
	RectangleShape bg;
	Texture tex;
	tex.loadFromFile("Graphics/bg.png");
	bg.setTexture(&tex);
	bg.setSize(Vector2f(window.getSize().x,window.getSize().y));
	RectangleShape logo(Vector2f(window.getSize().x * 0.45, window.getSize().y * 0.15));
	Texture log;
	logo.setPosition(Vector2f(window.getSize().x*0.27,20));
	log.loadFromFile("Graphics/unique.png");
	logo.setTexture(&log);
	Button NewGame("Graphics/newgame.png", Vector2f(112*2, 37 * 2), Vector2f(window.getSize().x / 2, (window.getSize().y / 2) -100));
	Button LoadGame("Graphics/LoadGame.png", Vector2f(112*2, 37*2), Vector2f(window.getSize().x/2 , (window.getSize().y / 2)));
	Button Settings("Graphics/settings.png", Vector2f(112 * 2, 37 *2), Vector2f(window.getSize().x / 2, (window.getSize().y / 2) + 100));
	Button Exit("Graphics/quit.png", Vector2f(112 * 2, 37 * 2), Vector2f(window.getSize().x / 2, (window.getSize().y / 2) + 200));

	//sfx volume, sound volume
	int sfx = 50;  
	int vol = 50;
	while (window.isOpen()) {
		Event event;

		mouse = window.mapPixelToCoords(Mouse::getPosition(window));

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
		
		//Load Game
		if (LoadGame.bounds().contains(mouse)) {
			LoadGame.Hover();
			if (Mouse::isButtonPressed(Mouse::Left)) {
				LoadGame.Click();
			}
			if (event.type == Event::MouseButtonReleased) {
				LoadState(window, mouse,dir); //load game
			}
		}

		//Settings
		else if (Settings.bounds().contains(mouse))
		{
			Settings.Hover();
			if (Mouse::isButtonPressed(Mouse::Left)) {
				Settings.Click();
			}
			if (event.type == Event::MouseButtonReleased) {
				SettingState(window,mouse,sfx,vol);
			}
			

		}

		//Exit
		else if (Exit.bounds().contains(mouse))
		{
			Exit.Hover();
			if (Mouse::isButtonPressed(Mouse::Left)) {
				Exit.Click();
			}
			if (event.type == Event::MouseButtonReleased) {
				window.close(); //exit
			}
		}

		//new game
		else if (NewGame.bounds().contains(mouse))
		{
			NewGame.Hover();
			if (Mouse::isButtonPressed(Mouse::Left)) {
				NewGame.Click();
			}
			if (event.type == Event::MouseButtonReleased) {
				newGame(window);
			}
		}

		//default
		else {
			LoadGame.Def();
			NewGame.Def();
			Exit.Def();
			Settings.Def();
		}
			



			window.clear();
			window.draw(bg);
			window.draw(logo);
			NewGame.Draw(window);
			LoadGame.Draw(window);
			Settings.Draw(window);
			Exit.Draw(window);
			window.display();	
		
	}
}