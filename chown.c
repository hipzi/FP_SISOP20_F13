#include "types.h"
#include "stat.h"
#include "user.h"

int 
fchown(char* file_name, uint new_UIDNumb, uint new_GID){

	if(getuid() == 0){

		return chown(file_name, new_UIDNumb, new_GID);
	}else{
		printf(1, "Invalid Permissions must be root\n");
		return -3;
	}
	return 0;
}

int
main(int argc, char **argv)
{
	if(argc != 3){
		printf(1, "Please give a file, and a User ID\n");
		exit();
	}
        char* owner = argv[1];
	char* group;
  	if ((group = strchr(argv[1], ':'))) {
    		*group = 0;
    		group++;
  	}
	if(group){
		int x = fchown(argv[2], atoi(owner), atoi(group));
		if(-1 == x){
			printf(1, "Error: Not a valid UID\n");
		}
		if(-2 == x){
			printf(1, "Error: File Name Invalid\n");
		}
		if (0 == x)
		{
			printf(1,"%s%s%s%s%s%s\n", "You changed ", argv[2], " owner ", owner, " group ", group);
		}
	}
	else {
		int x = fchown(argv[2], atoi(owner), -1);
		if(-1 == x){
			printf(1, "Error: Not a valid UID\n");
		}
		if(-2 == x){
			printf(1, "Error: File Name Invalid\n");
		}
		if (0 == x)
		{
			printf(1,"%s%s%s%s\n", "You changed ", argv[2], " owner ", owner);
		}
	}
	
 	 exit();
}
