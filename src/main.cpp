#include "smart_cat.h"

void displayBackground();

void displayLoading();

void drawDestopIcons();

void viewDesktopMode();

void playPongMode();

void moveMouseMode();

void customPrintStringZone(String str, int x, int y, int lineSpacing);

void printHttpPageContent(String link);

String getHttpsPagePayload(String link);

void getHttpsPageStream(String link);

void print_time();

const char *ssid = "Livebox-75C0";
const char *password = "ipW2j3EzJQg6LF9Er6";

//HttpClient httpClient__;
WiFiClient espClient;
WiFiUDP ntpUDP;
//HTTPClient httpClient;
HTTPClient httpsClient;

//NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)

// SDA => D2
// SCK => D1
Adafruit_SSD1306 display_1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 display_2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES 10 // Number of snowflakes in the animation example

std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);

void setup()
{
	Serial.begin(115200);
		Serial.setTimeout(2000);
	// Wire.begin(1, 15);
	//  second_wire.begin(9,10);
	// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
	if (!display_1.begin(SSD1306_SWITCHCAPVCC, 0x3C))
	{
		Serial.println(F("SSD1306 1 allocation failed"));
		for (;;)
			; // Don't proceed, loop forever
	}
	if (!display_2.begin(SSD1306_SWITCHCAPVCC, 0x3D))
	{
		Serial.println(F("SSD1306 2 allocation failed"));
		for (;;)
			; // Don't proceed, loop forever
	}

	display_1.setTextSize(1);	   // Normal 1:1 pixel scale
	display_1.setTextColor(WHITE); // Draw white text
	display_1.setCursor(0, 0);	   // Start at top-left corner
	display_1.cp437(true);

	display_2.setTextSize(1);	   // Normal 1:1 pixel scale
	display_2.setTextColor(WHITE); // Draw white text
	display_2.setCursor(0, 0);	   // Start at top-left corner
	display_2.cp437(true);

	// Show initial display buffer contents on the screen --
	// the library initializes this with an Adafruit splash screen.
	display_1.display();
	display_2.display();
	//delay(1000); // Pause for 2 seconds

	// Clear the buffer
	display_1.clearDisplay();
	display_2.clearDisplay();
		display_1.display();
	display_2.display();

	WiFi.begin(ssid, password);

	//display.drawBitmap(30, 30, mac_logo2_bmp, LOGO2_WIDTH, LOGO2_HEIGHT, SSD1306_WHITE);

	//display.println("Connecting to WiFi.");
	//display.display();
	Serial.println("Connecting to WiFi.");
	int _try = 0;
	while (WiFi.status() != WL_CONNECTED)
	{
		Serial.print("..");
		// displayLoading();
		display_1.print(".");
		display_2.print(".");
		display_1.display();
		display_2.display();
		delay(1000);
		_try++;
		if (_try >= NB_TRYWIFI)
		{
			display_1.clearDisplay();
			display_2.clearDisplay();
			Serial.println("Impossible to connect WiFi network, go to deep sleep");
			display_1.println("Impossible to connect WiFi network, go to deep sleep");
			display_2.println("Impossible to connect WiFi network, go to deep sleep");
			display_1.display();
			display_2.display();
		}
	}
	// while (_try < 5)
	// {
	// 	displayLoading();
	// 	delay(600);
	// 	_try++;
	// }
	display_1.clearDisplay();
	display_2.clearDisplay();
	display_1.setCursor(0, 0);	   // Start at top-left corner
	display_2.setCursor(0, 0);	   // Start at top-left corner
	
	display_1.println(WiFi.localIP());
	display_1.println(WiFi.macAddress());
	display_1.println(WiFi.gatewayIP());
	display_1.println(WiFi.RSSI());
	display_1.println(WiFi.hostname());


	display_2.println(WiFi.localIP());
	display_2.println(WiFi.macAddress());
	display_2.println(WiFi.gatewayIP());
	display_2.println(WiFi.RSSI());
	display_2.println(WiFi.hostname());
	display_1.display();
	display_2.display();
	delay(2000);



	//display.clearDisplay();
	Serial.println("Connected to the WiFi network");
	//display.println("Connected to the WiFi network");
	//display.display();
	// Draw a single pixel in white
	// display.drawPixel(10, 10, WHITE);

	// Show the display buffer on the screen. You MUST call display() after
	// drawing commands to make them visible on screen!
	//display.display();
	//delay(2000);
	// display.display() is NOT necessary after every single drawing command,
	// unless that's what you want...rather, you can batch up a bunch of
	// drawing operations and then update the screen all at once by calling
	// display.display(). These examples demonstrate both approaches...
	//display.clearDisplay();
	//display.println("NTP client starting...");
	Serial.println("NTP client starting...");
	//display.display();
	displayLoading();
	NTP.begin("pool.ntp.org", 1, true);
	NTP.setInterval(1000);
	//timeClient.begin();
	//display.clearDisplay();
	//display.println("NTP client started!");
	Serial.println("NTP client started!");
	display_1.display();
	display_2.display();

	client->setInsecure();

	/*httpClient.begin("http://www.arduino.cc/asciilogo.txt");
	int httpCode = httpClient.GET();
	Serial.printf("Http code :%d\n", httpCode);
	if (httpCode > 0)
	{
		if (httpCode == 200)//OK code
		{
			String payload = httpClient.getString();
			Serial.println(payload);
		}else {
					 Serial.printf("[HTTP] GET... failed, error: %s\n", httpClient.errorToString(httpCode).c_str());
			 }
	}else {
					 Serial.printf("[HTTP] GET... failed, error: %s\n", httpClient.errorToString(httpCode).c_str());
			 }
	
//  client.get("http://www.arduino.cc/asciilogo.txt");
}*/

	/*
NTP.onNTPSyncEvent([](NTPSyncEvent_t error) {
	if (error) {
		Serial.print("Time Sync error: ");
		if (error == noResponse)
			Serial.println("NTP server not reachable");
		else if (error == invalidAddress)
			Serial.println("Invalid NTP server address");
		}
	else {
		Serial.print("Got NTP time: ");
		Serial.println(NTP.getTimeDateString(NTP.getLastNTPSync()));
	}
});
*/
}

// 0 => blink
// 1 => open
// 2 => close
void blink_eyes(uint8_t eyes, uint8_t mode)
{
	uint8_t mid = 40;
	uint8_t max_steps = 5;

	if (mode == 0 || mode == 2)
	{

		for (uint8_t i = 0; i < max_steps; i++)
		{
			if (eyes & (1 << 0))
			{
				display_1.drawBitmap(0, 0, left_normal_bmp, RIGHT_NORMAL_WIDTH, RIGHT_NORMAL_HEIGHT, SSD1306_WHITE);
				display_1.fillRect(0, 0, 128, map(i, 0, max_steps, 0, mid), SSD1306_BLACK);
				display_1.fillRect(0, map(i, 0, max_steps, 64, mid), 128, 64 - map(i, 0, max_steps, 64, mid), SSD1306_BLACK);
				display_1.display();
			}
			if (eyes & (1 << 1))
			{
				display_2.drawBitmap(0, 0, right_normal_bmp, RIGHT_NORMAL_WIDTH, RIGHT_NORMAL_HEIGHT, SSD1306_WHITE);
				display_2.fillRect(0, 0, 128, map(i, 0, max_steps, 0, mid), SSD1306_BLACK);
				display_2.fillRect(0, map(i, 0, max_steps, 64, mid), 128, 64 - map(i, 0, max_steps, 64, mid), SSD1306_BLACK);
				display_2.display();
			}
		}
	}
	if (mode == 0 || mode == 1)
	{

		for (uint8_t i = max_steps; i > 0; i--)
		{
			if (eyes & (1 << 0))
			{
				display_1.drawBitmap(0, 0, left_normal_bmp, RIGHT_NORMAL_WIDTH, RIGHT_NORMAL_HEIGHT, SSD1306_WHITE);
				display_1.fillRect(0, 0, 128, map(i, 0, max_steps, 0, mid), SSD1306_BLACK);
				display_1.fillRect(0, map(i, 0, max_steps, 64, mid), 128, 64 - map(i, 0, max_steps, 64, mid), SSD1306_BLACK);
				display_1.display();
			}
			if (eyes & (1 << 1))
			{
				display_2.drawBitmap(0, 0, right_normal_bmp, RIGHT_NORMAL_WIDTH, RIGHT_NORMAL_HEIGHT, SSD1306_WHITE);
				display_2.fillRect(0, 0, 128, map(i, 0, max_steps, 0, mid), SSD1306_BLACK);
				display_2.fillRect(0, map(i, 0, max_steps, 64, mid), 128, 64 - map(i, 0, max_steps, 64, mid), SSD1306_BLACK);
				display_2.display();
			}
		}
		display_1.drawBitmap(0, 0, left_normal_bmp, RIGHT_NORMAL_WIDTH, RIGHT_NORMAL_HEIGHT, SSD1306_WHITE);
		display_1.display();
		display_2.drawBitmap(0, 0, right_normal_bmp, RIGHT_NORMAL_WIDTH, RIGHT_NORMAL_HEIGHT, SSD1306_WHITE);
		display_2.display();
	}
}

void wait_for_sun()
{

	while(true)
	{
		struct tm test = {0};
		sscanf(NTP.getTimeDateString().c_str(), "%d:%d:%d %d/%d/%d",
			&test.tm_hour, &test.tm_min, &test.tm_sec, &test.tm_mday, &test.tm_mon, &test.tm_year);
		// Serial.printf("h:%d, m:%d, s:%d    d:%d, m:%d, y%d\n", test.tm_hour, test.tm_min, test.tm_sec,
		// 			  test.tm_mday, test.tm_mon, test.tm_year);
		test.tm_isdst = -1; // Assume local daylight setting per date/time
		test.tm_mon--;		// Months since January
		if (test.tm_year >= 0 && test.tm_year < 100)
		{
			test.tm_year += 2000;
		}
		test.tm_year -= 1900; // Years since 1900
		time_t currentTime = mktime(&test);
		if ((hour(currentTime) >= 21 && hour(currentTime) <= 23) || hour(currentTime) < 6)
		{
			display_1.clearDisplay();
			display_2.clearDisplay();
			display_1.display();
			display_2.display();
			delay(60000);
		}
		else
		{
			break;
		}
	}
}

void loop()
{
	display_1.clearDisplay();
	display_2.clearDisplay();
	displayBackground();
	// printHttpPageContent("https://juthomas.github.io/test_site_web/micro-pc-content");
	display_1.display();
	display_2.display();
	blink_eyes((1 << 0) | (1 << 1), 2);

	display_1.clearDisplay();
	display_2.clearDisplay();
	display_1.display();
	display_2.display();
	delay(100);


	display_1.clearDisplay();
	display_2.clearDisplay();
	print_time();
	display_1.display();
	display_2.display();
	delay(3000);
	display_1.clearDisplay();
	display_2.clearDisplay();
	display_1.display();
	display_2.display();
	blink_eyes((1 << 0) | (1 << 1), 1);
	display_1.clearDisplay();
	display_2.clearDisplay();
	displayBackground();
	display_1.display();
	display_2.display();
	delay(7000);

	wait_for_sun();


}

void displayBackground()
{
	// display.fillRect(0, 0, display.width(), 10, SSD1306_WHITE);
	// display.fillRect(0, 11, display.width(), display.height(), SSD1306_WHITE);
	// display.drawBitmap(0, 0, right_normal_bmp, RIGHT_NORMAL_WIDTH, RIGHT_NORMAL_HEIGHT, SSD1306_WHITE);
	display_1.drawBitmap(0, 0, left_normal_bmp, RIGHT_NORMAL_WIDTH, RIGHT_NORMAL_HEIGHT, SSD1306_WHITE);
	display_2.drawBitmap(0, 0, right_normal_bmp, RIGHT_NORMAL_WIDTH, RIGHT_NORMAL_HEIGHT, SSD1306_WHITE);
	// display.setTextSize(1);		 // Normal 1:1 pixel scale
	// display.setTextColor(BLACK); // Draw white text
	// display.setCursor(13, 1);
	// display.cp437(true);
	// display.print("File Edit");
}

void displayLoading()
{
	// display.clearDisplay();
	// static int loadingStep = 0;

	// // display.drawBitmap(64 - 8, 20, mac_logo2_bmp, LOGO2_WIDTH, LOGO2_HEIGHT, SSD1306_WHITE);

	// for (int i = 0; i < loadingStep; i++)
	// {
	// 	// display.drawBitmap(64 - 28 + 12 * i, 50, mac_logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE);
	// }
	// display.display();
	// loadingStep++;
	// if (loadingStep > 5)
	// {
	// 	loadingStep = 0;
	// }
}

void drawDestopIcons()
{
	// display.drawBitmap(115, 15, floppy_logo_bmp, FLOPPY_LOGO_WIDTH, FLOPPY_LOGO_HEIGHT, SSD1306_BLACK);
	// display.drawBitmap(115, 25, fake_text_bmp, FAKE_TEXT_WIDTH, FAKE_TEXT_HEIGHT, SSD1306_BLACK);

	// display.drawBitmap(100, 15, folder_logo_bmp, FOLDER_LOGO_WIDTH, FOLDER_LOGO_HEIGHT, SSD1306_BLACK);
	// display.drawBitmap(100, 25, fake_text_bmp, FAKE_TEXT_WIDTH, FAKE_TEXT_HEIGHT, SSD1306_BLACK);

	// display.drawBitmap(100, 30, folder_logo_bmp, FOLDER_LOGO_WIDTH, FOLDER_LOGO_HEIGHT, SSD1306_BLACK);
	// display.drawBitmap(100, 40, fake_text_bmp, FAKE_TEXT_WIDTH, FAKE_TEXT_HEIGHT, SSD1306_BLACK);

	// display.drawBitmap(115, 30, folder_logo_bmp, FOLDER_LOGO_WIDTH, FOLDER_LOGO_HEIGHT, SSD1306_BLACK);
	// display.drawBitmap(115, 40, fake_text_bmp, FAKE_TEXT_WIDTH, FAKE_TEXT_HEIGHT, SSD1306_BLACK);

	// display.drawBitmap(115, 45, trash_logo_bmp, TRASH_LOGO_WIDTH, TRASH_LOGO_HEIGHT, SSD1306_BLACK);
	// display.drawBitmap(115, 55, fake_text_bmp, FAKE_TEXT_WIDTH, FAKE_TEXT_HEIGHT, SSD1306_BLACK);
}

void viewDesktopMode()
{

	drawDestopIcons();
	// display.drawBitmap(30, 50, mouse_bmp, MOUSE_WIDTH, MOUSE_HEIGHT, SSD1306_BLACK);
}

void displayTimeMode()
{
	// display.drawLine(0, 18, display.width(), 18, SSD1306_BLACK);
	// display.drawRect(1, 12, 5, 5, SSD1306_BLACK);

	// display.drawLine(8, 12, 40, 12, SSD1306_BLACK);
	// display.drawLine(8, 14, 40, 14, SSD1306_BLACK);
	// display.drawLine(8, 16, 40, 16, SSD1306_BLACK);

	// // display.drawBitmap(44, 12, date_title_bmp, DATE_TITLE_WIDTH, DATE_TITLE_HEIGHT, SSD1306_BLACK);

	// display.drawLine(81, 12, 113, 12, SSD1306_BLACK);
	// display.drawLine(81, 14, 113, 14, SSD1306_BLACK);
	// display.drawLine(81, 16, 113, 16, SSD1306_BLACK);

	// display.drawRect(116, 12, 5, 5, SSD1306_BLACK);
	// display.drawRect(116, 12, 3, 3, SSD1306_BLACK);
	// display.drawRect(122, 12, 5, 5, SSD1306_BLACK);
	// display.drawLine(122, 14, 126, 14, SSD1306_BLACK);

	// display.drawLine(120, 19, 120, display.height(), SSD1306_BLACK);
	// display.fillRect(122, 20, 5, 17, SSD1306_BLACK);
	// display.drawLine(120, 48, 128, 48, SSD1306_BLACK);
	// display.drawLine(120, 56, 128, 56, SSD1306_BLACK);
	// display.drawBitmap(121, 51, arrow_up_bmp, ARROW_UP_WIDTH, ARROW_UP_HEIGHT, SSD1306_BLACK);
	// display.drawBitmap(121, 58, arrow_down_bmp, ARROW_DOWN_WIDTH, ARROW_DOWN_HEIGHT, SSD1306_BLACK);
	// display.drawBitmap(100, 52, mouse_bmp, MOUSE_WIDTH, MOUSE_HEIGHT, SSD1306_BLACK);

	// display.setTextSize(1);		 // Normal 1:1 pixel scale
	// display.setTextColor(BLACK); // Draw white text
	// Start at top-left corner
	//display.setCursor(10, 1);     // Start at top-left corner
	// display.cp437(true);
	//display.println(F("time :"));

	// Serial.println(NTP.getTimeStr());
	// time_t currentTime = NTP.getTime();
	struct tm test = {0};
	sscanf(NTP.getTimeDateString().c_str(), "%d:%d:%d %d/%d/%d",
		   &test.tm_hour, &test.tm_min, &test.tm_sec, &test.tm_mday, &test.tm_mon, &test.tm_year);
	// Serial.printf("h:%d, m:%d, s:%d    d:%d, m:%d, y%d\n", test.tm_hour, test.tm_min, test.tm_sec,
	// 			  test.tm_mday, test.tm_mon, test.tm_year);
	test.tm_isdst = -1; // Assume local daylight setting per date/time
	test.tm_mon--;		// Months since January
	if (test.tm_year >= 0 && test.tm_year < 100)
	{
		test.tm_year += 2000;
	}
	test.tm_year -= 1900; // Years since 1900
	time_t currentTime = mktime(&test);
	// display.setCursor(6, 28);
	// display.printf("%s,", dayStr(dayOfWeek(currentTime)));
	// display.setCursor(6, 39);
	// display.printf("%d", day(currentTime));
	// if (day(currentTime) == 1 || day(currentTime) == 21 || day(currentTime) == 31)
	// {
	// 	display.print("st");
	// }
	// else if (day(currentTime) == 2 || day(currentTime) == 22)
	// {
	// 	display.print("nd");
	// }
	// else if (day(currentTime) == 3 || day(currentTime) == 23)
	// {
	// 	display.print("rd");
	// }
	// else
	// {
	// 	display.print("th");
	// }
	// display.printf(" %s %04d", monthStr(month(currentTime)), year(currentTime));

	// display.println(NTP.getTimeDateString());
	// display.println(NTP.getDateStr());
	// display.println(NTP.getTimeStr());
}

void playPongMode()
{
	// display.drawBitmap(60, 14, pong_middle_bmp, PONG_MIDDLE_WIDTH, PONG_MIDDLE_HEIGHT, SSD1306_BLACK);
	// display.drawBitmap(5, 40, pong_player_bmp, PONG_PLAYER_WIDTH, PONG_PLAYER_HEIGHT, SSD1306_BLACK);
	// display.drawBitmap(116, 30, pong_player_bmp, PONG_PLAYER_WIDTH, PONG_PLAYER_HEIGHT, SSD1306_BLACK);
	// display.drawBitmap(30, 30, pong_ball_bmp, PONG_BALL_WIDTH, PONG_BALL_HEIGHT, SSD1306_BLACK);

	// static int y_actual = 30;
	// int y_desired;
	// int y_current;
	// int x_current;
	// int x_min = 8 + 2;
	// int x_max = 128 - 8 - 2 - 3;
	// static bool playerTurn = false;
	// static int y_player_1 = 30 - 7;
	// static int y_player_1_old = 30;
	// static int y_player_2 = 30 - 7;
	// static int y_player_2_old = 30;

	// for (int positions = 0; positions < 20; positions++)
	// {
	// 	y_desired = random(17, 55);
	// 	for (int percentage = 0; percentage < 100; percentage++)
	// 	{
	// 		x_current = map(percentage, playerTurn == true ? 0 : 100, playerTurn == true ? 100 : 0, x_min, x_max);

	// 		y_current = map(percentage, 0, 100, y_actual, y_desired);
	// 		display.clearDisplay();
	// 		displayBackground();
	// 		print_time();

	// 		if (!playerTurn)
	// 		{
	// 			y_player_1 = map(percentage, 0, 100, y_player_1_old, y_desired) - 7;
	// 		}
	// 		else
	// 		{
	// 			y_player_2 = map(percentage, 0, 100, y_player_2_old, y_desired) - 7;
	// 		}
	// 		//drawDestopIcons();
	// 		//display.drawBitmap(x_current, y_current, mouse_bmp, MOUSE_WIDTH, MOUSE_HEIGHT, SSD1306_BLACK);
	// 		// display.drawBitmap(60, 14, pong_middle_bmp, PONG_MIDDLE_WIDTH, PONG_MIDDLE_HEIGHT, SSD1306_BLACK);
	// 		// display.drawBitmap(5, y_player_1, pong_player_bmp, PONG_PLAYER_WIDTH, PONG_PLAYER_HEIGHT, SSD1306_BLACK);
	// 		// display.drawBitmap(116, y_player_2, pong_player_bmp, PONG_PLAYER_WIDTH, PONG_PLAYER_HEIGHT, SSD1306_BLACK);
	// 		// display.drawBitmap(x_current, y_current, pong_ball_bmp, PONG_BALL_WIDTH, PONG_BALL_HEIGHT, SSD1306_BLACK);
	// 		display.display();
	// 	}

	// 	if (!playerTurn)
	// 	{
	// 		y_player_1_old = y_desired;
	// 	}
	// 	else
	// 	{
	// 		y_player_2_old = y_desired;
	// 	}
	// 	y_actual = y_desired;
	// 	playerTurn = playerTurn == true ? false : true;
	// }
}

void moveMouseMode()
{
	// static int x_actual = 30;
	// static int y_actual = 30;
	// int x_desired;
	// int y_desired;
	// int x_current;
	// int y_current;

	// for (int positions = 0; positions < 10; positions++)
	// {
	// 	x_desired = random(1, display.width() - 8);
	// 	y_desired = random(1, display.height() - 8);
	// 	for (int percentage = 0; percentage < 100; percentage++)
	// 	{
	// 		x_current = map(percentage, 0, 100, x_actual, x_desired);
	// 		y_current = map(percentage, 0, 100, y_actual, y_desired);

	// 		display.clearDisplay();
	// 		displayBackground();
	// 		print_time();
	// 		drawDestopIcons();
	// 		// display.drawBitmap(x_current, y_current, mouse_bmp, MOUSE_WIDTH, MOUSE_HEIGHT, SSD1306_BLACK);
	// 		display.display();
	// 	}
	// 	x_actual = x_desired;
	// 	y_actual = y_desired;
	// }
}

void customPrintStringZone(String str, int x, int y, int lineSpacing)
{
	// String tmpStr = "";

	// if (str.indexOf("[{") != -1 && str.indexOf("}]") != -1 && str.indexOf("[{") < str.indexOf("}]"))
	// {
	// 	tmpStr = str.substring(str.indexOf("[{") + 2, str.indexOf("}]"));
	// 	Serial.printf("Http request : ");
	// 	String pageContent = getHttpsPagePayload(tmpStr);
	// 	display.setCursor(x, y);
	// 	display.print(pageContent);

	// 	Serial.println(pageContent);
	// 	return;
	// }
	// else if (str.indexOf("[VIEW_DESKTOP]") != -1)
	// {
	// 	viewDesktopMode();
	// 	return;
	// }
	// else if (str.indexOf("[PLAY_PONG]") != -1)
	// {
	// 	playPongMode();
	// 	return;
	// }
	// else if (str.indexOf("[MOVE_MOUSE]") != -1)
	// {
	// 	moveMouseMode();
	// 	return;
	// }
	// else if (str.indexOf("[DISPLAY_TIME]") != -1)
	// {
	// 	displayTimeMode();
	// 	return;
	// }

	// while (str != "")
	// {
	// 	if (str.indexOf("\n") != -1)
	// 	{
	// 		tmpStr = str.substring(0, str.indexOf("\n"));
	// 		str = str.substring(str.indexOf("\n") + 1);

	// 		Serial.println("2");
	// 	}
	// 	else
	// 	{
	// 		Serial.println("1");
	// 		tmpStr = str;
	// 		str = "";
	// 	}
	// 	Serial.println("--");
	// 	Serial.println(tmpStr);
	// 	Serial.println("--");
	// 	Serial.println(str);
	// 	Serial.println("--");
	// 	display.setCursor(x, y);
	// 	display.print(tmpStr);
	// 	y += lineSpacing;
	// }
}

void printHttpPageContent(String link)
{
	// // https://raw.githubusercontent.com/juthomas/Macintosh_SE_tiny_ESP8266/master/se_state
	// String textString = getHttpsPagePayload("https://raw.githubusercontent.com/juthomas/Macintosh_SE_tiny_ESP8266/master/se_state");
	// // String textString = getHttpsPagePayload("https://juthomas.github.io/test_site_web/micro-pc-content");
	// //display.clearDisplay();
	// display.setTextSize(1);		 // Normal 1:1 pixel scale
	// display.setTextColor(BLACK); // Draw white text
	// display.setCursor(1, 12);	 // Start at top-left corner
	// display.cp437(true);

	// customPrintStringZone(textString, 1, 12, 9);
	// //display.print(textString);
}

String getHttpsPagePayload(String link)
{
	// String payload = "";

	// Serial.print("[HTTPS] begin...\n");
	// Serial.print("Link Requested :");
	// Serial.println(link);
	// if (httpsClient.begin(*client, link))
	// { // HTTPS

	// 	Serial.print("[HTTPS] GET...\n");
	// 	// start connection and send HTTP header
	// 	int httpCode = httpsClient.GET();

	// 	// httpCode will be negative on error
	// 	if (httpCode > 0)
	// 	{
	// 		// HTTP header has been send and Server response header has been handled
	// 		Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

	// 		// file found at server
	// 		if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
	// 		{
	// 			String httpsContent = F("");
	// 			int len = httpsClient.getSize();
	// 			if ((uint32_t)len < ESP.getFreeHeap())
	// 			{

	// 				payload = httpsClient.getString();
	// 				Serial.println(payload);

	// 				//String payload = httpsClient.getString();
	// 				//Serial.println(payload);
	// 			}
	// 			else
	// 			{
	// 				Serial.printf("Only %do of ram left need %do", ESP.getFreeHeap(), len);
	// 			}
	// 		}
	// 	}
	// 	else
	// 	{
	// 		Serial.printf("[HTTPS] GET... failed, error: %s\n", httpsClient.errorToString(httpCode).c_str());
	// 	}

	// 	httpsClient.end();
	// }
	// else
	// {
	// 	Serial.printf("[HTTPS] Unable to connect\n");
	// }
	// return (payload);
}

void getHttpsPageStream(String link)
{
	Serial.print("[HTTPS] begin...\n");
	if (httpsClient.begin(*client, link))
	{ // HTTPS

		Serial.print("[HTTPS] GET...\n");
		// start connection and send HTTP header
		int httpCode = httpsClient.GET();

		// httpCode will be negative on error
		if (httpCode > 0)
		{
			// HTTP header has been send and Server response header has been handled
			Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

			// file found at server
			if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
			{
				String httpsContent = F("");
				int len = httpsClient.getSize();
				if ((uint32_t)len < ESP.getFreeHeap())
				{

					Serial.printf("Https page size : %d\n", len);
					static uint8_t buff[128] = {0};
					while (httpsClient.connected() && (len > 0 || len == -1))
					{
						// get available data size
						size_t size = client->available();

						if (size)
						{
							// read up to 128 byte
							int c = client->readBytes(buff, ((size > sizeof(buff) - 1) ? sizeof(buff) - 1 : size));

							// write it to Serial
							//Serial.write(buff, c);
							Serial.println(ESP.getFreeHeap(), DEC);
							Serial.printf("Prepare to feed httpsContent, left %d\n", len);
							httpsContent += ((const __FlashStringHelper *)buff);
							;

							//httpsContent += toCharArray(buff, c);
							if (len > 0)
							{
								len -= c;
							}
						}
						delay(1);
					}
					Serial.println("https content : ");
					Serial.println(httpsContent);

					//String payload = httpsClient.getString();
					//Serial.println(payload);
				}
				else
				{
					Serial.printf("Only %do of ram left need %do", ESP.getFreeHeap(), len);
				}
			}
		}
		else
		{
			Serial.printf("[HTTPS] GET... failed, error: %s\n", httpsClient.errorToString(httpCode).c_str());
		}

		httpsClient.end();
	}
	else
	{
		Serial.printf("[HTTPS] Unable to connect\n");
	}
}

void print_time()
{




	display_1.setTextSize(2);		 // Normal 1:1 pixel scale
	display_2.setTextSize(4);		 // Normal 1:1 pixel scale
	display_1.setTextColor(WHITE); // Draw white text
	display_2.setTextColor(WHITE); // Draw white text
		 // Start at top-left corner
	//display.setCursor(10, 1);     // Start at top-left corner
	display_1.cp437(true);
	display_2.cp437(true);
	//display.println(F("time :"));

	// Serial.println(NTP.getTimeStr());
	// time_t currentTime = NTP.getTime();
	struct tm test = {0};
	sscanf(NTP.getTimeDateString().c_str(), "%d:%d:%d %d/%d/%d",
		   &test.tm_hour, &test.tm_min, &test.tm_sec, &test.tm_mday, &test.tm_mon, &test.tm_year);
	// Serial.printf("h:%d, m:%d, s:%d    d:%d, m:%d, y%d\n", test.tm_hour, test.tm_min, test.tm_sec,
	// 			  test.tm_mday, test.tm_mon, test.tm_year);
	test.tm_isdst = -1; // Assume local daylight setting per date/time
	test.tm_mon--;		// Months since January
	if (test.tm_year >= 0 && test.tm_year < 100)
	{
		test.tm_year += 2000;
	}
	test.tm_year -= 1900; // Years since 1900
	time_t currentTime = mktime(&test);
	display_1.setCursor(6, 5);
	display_1.printf("%s,", dayStr(dayOfWeek(currentTime)));
	display_1.setCursor(6, 25);
	display_1.printf("%d", day(currentTime));
	if (day(currentTime) == 1 || day(currentTime) == 21 || day(currentTime) == 31)
	{
		display_1.print("st");
	}
	else if (day(currentTime) == 2 || day(currentTime) == 22)
	{
		display_1.print("nd");
	}
	else if (day(currentTime) == 3 || day(currentTime) == 23)
	{
		display_1.print("rd");
	}
	else
	{
		display_1.print("th");
	}
	display_1.setCursor(6, 45);
	display_1.printf("%s %04d", monthStr(month(currentTime)), year(currentTime));
	display_2.setCursor(5, 20);

	display_2.printf("%02d:%02d", hour(currentTime), minute(currentTime));
	// display_2.printf("%s:%s", hour(currentTime), minute(currentTime));
}