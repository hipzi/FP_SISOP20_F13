#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

char *fmtname(char *path){
	static char buf[DIRSIZ+1];

	char *p;

	for(p = path + strlen(path);p>=path && *p != '/';p--);
	p++;

	if(strlen(p) >= DIRSIZ)
		return p;
	memmove(buf,p,strlen(p));
	memset(buf+strlen(p),' ',DIRSIZ-strlen(p));
	return buf;
}

int strstr(char *haystack, char *needle){
    int n = strlen(haystack);
	int m = strlen(needle);

	int i, j;
	for(i=0;i<=n-m;i++){

		for(j=0;j<m;j++){
			if(haystack[i+j] != needle[j]) 
                break;
		}

		if(j==m)return 1;
	}
	return 0;
}

int strncmp(const char *p, const char *q, uint n)
{
  while(n > 0 && *p && *p == *q)
    n--, p++, q++;
  if(n == 0)
    return 0;
  return (uchar)*p - (uchar)*q;
}

void find(char *path, char *needle){
	char buf[512];
	struct dirent de;
	struct stat st;

	int dir = open(path,0);
	if(dir < 0){
		printf(2,"ls: cannot access '%s': No such file or directory\n",path);
		return;
	}

	if(fstat(dir,&st) < 0){
		printf(2,"ls: cannot stat '%s'\n",path);
		close(dir);
		return;
	}

	if(st.type == T_FILE){
		printf(1,"%s\n",fmtname(path));
	}
	else if(st.type == T_DIR){
		if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
			printf(1,"ls: path too long\n");
			return;
		}
		strcpy(buf,path);
		char *p = buf+strlen(buf);
		*p++ = '/';
		while(read(dir,&de,sizeof (de)) == sizeof (de)){
			if(de.inum == 0)continue;
			memmove(p,de.name,DIRSIZ);
			p[DIRSIZ] = 0;
			if(stat(buf,&st) < 0){
				printf(2,"ls: cannot stat '%s'\n",buf);
				continue;
			}
			if(strstr(fmtname(buf),needle)){
                if(strncmp(buf,"/",1)==0){
                    printf(1,"%s\n",fmtname(buf));
                }
                else printf(1,"%s/%s\n",path,fmtname(buf));
            }
            if(fmtname(buf)[0] != '.' && st.type == T_DIR){
                find(de.name, needle);
            }
		}
	}
	close(dir);
	return;
}

int main(int argc, char *argv[])
{
	if(argc < 3){
		printf(1,"find -n [File] \n");
        printf(1,"find -d [Directory] \n");
	}
    for(int i=1;i<argc-1;i++){
        if (argv[i][0] == '-' && argv[i][1] == 'n'){
            find("/",argv[i+1]);
        }
        if (argv[i][0] == '-' && argv[i][1] == 'd'){
            find("/",argv[i+1]);
        }
    }
	exit();
}
