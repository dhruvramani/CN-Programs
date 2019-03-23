#include<unistd.h>
#include<stdio.h>
#include<sys/select.h>
#include<sys/time.h>
#include<sys/types.h>

int main()
{
	int no = 4, fds[5];
	for(int i=0; i<no; i++)
	{
		char file_path[50];
		sprintf(file_path, "./p_%d", i);
		FILE *f = popen(file_path, "r");
		fds[i] = fileno(f);
	}
	// Read from other Ps and write to P4
	FILE *f4 = popen("./p_4","w");
	int fd4 = fileno(f4);

	int m=0; // Find the max Fid to specify range of rset bitmap to check (so doesn't check all 1024)
	for(int i=0; i<no; i++)
		if(m < fds[i])
			m = fds[i];
	
	fd_set rset; // Bitmap resembling open file pointers. 1 wherever FD exists - 1024 in total
	struct timeval timeout;
	while(1)
	{
		// Call set, select in a loop
		for(int i=0; i<no; i++)
			FD_SET(fds[i], &rset); // Sets the FD to bitmap
	
		timeout.tv_sec = 50; // MiliSeconds
		timeout.tv_usec = 0; // MicroSeconds
		int num = select(m+1, &rset, NULL, NULL, &timeout); // Returns number of bits which are set - when active.
		
		char buffer[100];
		for(int i=0; i<no; i++)
		{
			if(FD_ISSET(fds[i], &rset)) // If bitmap for that poss is set
			{
				printf("P%d is ready\n", fds[i]);
				read(fds[i], buffer, 100);
				write(fd4, buffer, 100);		
			}
		}
		FD_ZERO(&rset);
	}
	return 0;
}
