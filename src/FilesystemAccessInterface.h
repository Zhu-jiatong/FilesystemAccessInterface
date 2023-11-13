#if !defined(_FilesystemAccessInterface_h_)
#define _FilesystemAccessInterface_h

#include <Arduino.h>
#include <FS.h>
#include <AccessPermissionManager.h>

class FilesystemAccessInterface : public AccessPermissionManager
{
public:
	FilesystemAccessInterface(FS &fs) : _fs(fs) {}

	File openFile(const String &username, const String &path, const char *mode);
	void deleteFile(const String &username, const String &path);
	void renameFile(const String &username, const String &pathFrom, const String &pathTo);
	void createDirectory(const String &username, const String &path);

private:
	FS &_fs;
};

#endif // _FilesystemAccessInterface_h
