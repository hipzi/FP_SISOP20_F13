#include "types.h"
#include "stat.h"
#include "user.h"

int 
uchown(char* file_name, uint new_UIDNumb){

	if(new_UIDNumb == -1){
		printf(1, "%s UID is %d\n",file_name, chown(file_name, -1));
	}else{
		if(getuid() == 0){
			return chown(file_name, new_UIDNumb);
		}else{
			printf(1, "Invalid Permissions must be root\n");
			return -3;
		}
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
	if((int)*argv[1] == 63){
		uchown(argv[2], -1);
	}else{
		int x = uchown(argv[2], atoi(argv[1]));

		if(-1 == x){
			printf(1, "Error: Not a valid UID\n");
		}
		if(-2 == x){
			printf(1, "Error: File Name Invalid\n");
		}
		if (0 == x)
		{
			printf(1,"%s%s%s%s\n", "You changed owner ", argv[2], " to ", argv[1]);
		}
	}
	
 	 exit();
}
