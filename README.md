# GameLab Project Repository

|  General Info  | |
| ---|---|
| Working Title | My very cool Game Lab 3 project |
| Final Title | Edit me! |
| Student | Yannik Stamm, yannik.stamm@stud-mail.uni-wuerzburg.de, s431656 |
| Target Platform(s) | `Edit me!` |
| Start Date | 16.9.2023 |
| Study Program | Games Engineering B.Sc.|
| Engine Version | None (OpenGL?) |

### Abstract

`Insert your project description here, and keep it up-to-date.`

`--- 8< --- READ, THEN REPLACE WITH CUSTOM CONTENT BELOW HERE --- 8< ---`

## Repository Usage Guides

```
RepositoryRoot/
    ├── README.md           // This should reflect your project 
    │                       //  accurately, so always merge infor- 
    │                       //  mation from your concept paper 
    │                       //  with the readme
    ├── builds/             // Archives (.zip) of built executables of your projects
    │                       //  including (non-standard) dependencies
    ├── code/
    │   ├── engine/         // Place your project folder(s) here
    │   ├── my-game-1/      // No un-used folders, no "archived" folders
    │   ├── CMakeLists.txt  // e.g. if using CMake, this can be your project root
    │   └── ...
    ├── documentation/      // GL2/3 - Each project requires FULL documentation  
    │                       //   i.e. API Docs, Handbook, Dev Docs
    ├── poster/             // PDF of your Poster(s)
    ├── report/             // PDF
    └── trailer/            // .mp4 (final trailer, no raw material)
```

#### Versioning Binary Assets with Git LFS and .gitattributes
Gitattribute files define file types to be handled through the Git Large File Storage (Git LFS) System.
This system does not handle binary files, such as assets, images, meshes, etc. well.
Even minimal changes add the whole file to the projects history.
Git LFS identifies iterations of binary files using a hash in the repository, but stores the actual binary data transparently in a seperate data silo.

To let Git LFS track a certain file (e.g. recursively all *.jpg*), execute this command:

	> git lfs track *.jpg

This command creates the following entry in the *.gitattributes* file:

	*.jpg filter=lfs diff=lfs merge=lfs -text

Git LFS is installed on all Workstations in E37 and the GameLabs.
For your private computer, you can [download Git LFS here](https://git-lfs.github.com/).
