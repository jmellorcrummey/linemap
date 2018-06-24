class FileSystemRepr;

class FileSystem {
public:

  FileSystem();
  ~FileSystem();

  const char *getFileName(i);
  const char *getDirName(i);


private:
   FileSystemRepr *repr;
}
