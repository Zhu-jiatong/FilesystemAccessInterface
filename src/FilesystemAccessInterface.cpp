#include "FilesystemAccessInterface.h"
#include <FilesystemUtility.h>

File FilesystemAccessInterface::openFile(const String &username, const String &path, const char *mode)
{
	Permission permission;
	if (mode == FILE_READ)
		permission = Permission::READ;
	else if (mode == FILE_WRITE || mode == FILE_APPEND)
		permission = Permission::WRITE;
	else
		throw std::runtime_error(("Invalid mode: " + String(mode)).c_str());

	if (!canUserAccessFile(username, path, permission))
		throw std::runtime_error((username + " does not have " + (permission == Permission::READ ? "read" : "write") + " access to " + path).c_str());

	File file = _fs.open(path, mode);
	if (!file)
		throw std::runtime_error(("Failed to open file: " + path).c_str());

	return file;
}

void FilesystemAccessInterface::deleteFile(const String &username, const String &path)
{
	if (!canUserAccessFile(username, path, Permission::WRITE))
		throw std::runtime_error((username + " does not have write access to " + path).c_str());

	File root = _fs.open(path);
	if (!root)
		throw std::runtime_error(("Failed to open file: " + path).c_str());

	FSUtil::recursiveDelete(_fs, root);
}

void FilesystemAccessInterface::renameFile(const String &username, const String &pathFrom, const String &pathTo)
{
	if (!canUserAccessFile(username, pathFrom, Permission::WRITE) && canUserAccessFile(username, pathTo, Permission::WRITE))
		throw std::runtime_error((username + " does not have write access to " + pathFrom + " or " + pathTo).c_str());

	if (!_fs.rename(pathFrom, pathTo))
		throw std::runtime_error(("Failed to rename file: " + pathFrom + " to " + pathTo).c_str());
}

void FilesystemAccessInterface::createDirectory(const String &username, const String &path)
{
	String parent = FSUtil::parentPath(path);
	if (!canUserAccessFile(username, parent, Permission::WRITE))
		throw std::runtime_error((username + " does not have write access to " + parent).c_str());

	if (!_fs.mkdir(path))
		throw std::runtime_error(("Failed to create directory: " + path).c_str());
}