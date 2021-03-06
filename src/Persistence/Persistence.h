#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "List.h"
#include "String.h"

class Explorer;
class Persistence
{
public:
   static bool IsDir(const String &path);
   static bool IsFile(const String &path);
   static bool IsAbsolute(const String &path);

   static String GetDir(const String &filepath);
   static String GetFileName(const String &filepath);
   static String GetFileNameWithExtension(const String &filepath);
   static String GetFileExtensionFirst(const String &filepath);
   static String GetFileExtensionLast(const String &filepath);
   static String GetFileExtensionComplete(const String &filepath);
   static String GetPathWithoutExtension(const String &filepath);

   /**
    * ./Images/wololo => ${prependDirectory}/Images/wololo
    *   Images/wololo => ${prependDirectory}/Images/wololo
   **/
   static String ToAbsolute(const String &relPath,
                            const String &prependDirectory);
   static String ToAbsolute(const String &relPath,
                            bool isEngineFile);

   /**
    * ${prependDirectory}/Images/wololo => ./Images/wololo
   **/
   static String ToRelative(const String &absPath,
                            const String &prependDirectory);
   static String ToRelative(const String &relPath,
                            bool isEngineFile);
   static String ToRelative(const String &absPath);

   static bool IsEngineFile(const String &filepath);

   static String GetDirUp(const String &filepath);
   static String GetRightmostDir(const String &dir);

   static bool DuplicateFile(const String &fromFilepath,
                             const String &toFilepath,
                             bool overwrite = true);
   static bool DuplicateDir(const String &fromDirpath,
                            const String &toDirpath,
                            bool overwrite = true);

    #ifdef BANG_EDITOR
   /**
    * @brief wololo/omg/myFile.jpg => wololo/omg/myFile_1.jpg
    *        wololo/omg/myFile_2.jpg => wololo/omg/myFile_3.jpg
    *        etc.
    * @param path
    * @return
    */
   static String GetNextDuplicateName(const String &path);
   static String GetDuplicateName(const String &path);
    #endif

   static List<String> GetSubDirectories(const String& dirPath,
                                         bool recursive = false);
   static List<String> GetFiles(const String& dirPath,
                                bool recursive = false,
                                const List<String> &extensions = {});

   static bool Remove(const String &path);
   static bool ExistsFile(const String &filepath);
   static bool ExistsDirectory(const String &dirPath);
   static bool Exists(const String &filepath);
   static bool CreateDirectory(const String &dirPath);
   static void SetActiveSceneFilepath(const String &scenePath);
   static bool Rename(const String &oldPath, const String &newPath);
   static bool Move(const String &oldPath, const String &newPath);

   static String GetHash(const String &filepath);

   //Appends an extension to a filepath only if that filename does not contain the extension
   static String AppendExtension(const String &filepath, const String extNoDot);

   static void InitFromMainBinary();

   static Persistence *GetInstance();
   static const String& GetCurrentSceneFilepath();
   static const String& GetProjectRootAbs();
   static const String& GetProjectAssetsRootAbs();
   static const String& GetEngineRootAbs();
   static const String& GetEngineAssetsRootAbs();

private:
   String c_currentSceneFilepath      = "";
   String c_ProjectRootAbsolute       = "";
   String c_ProjectAssetsRootAbsolute = "";
   String c_EngineRootAbsolute        = "";
   String c_EngineAssetsRootAbsolute  = "";

   friend class ProjectManager;
   friend int main(int argc, char** argv);
};

#endif // PERSISTENCE_H
