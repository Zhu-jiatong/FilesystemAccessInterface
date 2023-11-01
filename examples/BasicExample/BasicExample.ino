#include "src/FilesystemAccessInterface.h"
#include "SD.h"

void setup()
{
	try
	{
		Serial.begin(115200);

		if (!SD.begin())
			throw std::runtime_error("SD card initialization failed!");

		FilesystemAccessInterface fs(SD);

		fs.open("/sd/test.db");

		File file = fs.openFile("testUser", "/testUser/testFile.txt", FILE_READ);
		Serial.println(file.readString());

		fs.shareFileWithUser("/testUser/testFile.txt", "testUser2", Permission::READ);
		fs.deleteFile("testUser2", "/testUser/testFile.txt");
		fs.deleteFile("testUser", "/testUser/testFile.txt");
	}
	catch (const std::exception &e)
	{
		Serial.println(e.what());
	}
}

void loop() {}