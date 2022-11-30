#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_NUM 30

int main()
{
	int fd1[2], fd2[2], fd3[2], fd4[2];

	char randomChar[2];
	int recordNum = 0;
	int logCount = 0;

	if (pipe(fd1) == -1)
	{
		fprintf(stderr, "Pipe Failed");
		return (1);
	}
	if (pipe(fd2) == -1)
	{
		fprintf(stderr, "Pipe Failed");
		return (1);
	}
	if (pipe(fd3) == -1)
	{
		fprintf(stderr, "Pipe Failed");

		return (1);
	}
	if (pipe(fd4) == -1)
	{
		fprintf(stderr, "Pipe Failed");
		return (1);
	}

	pid_t pidA, pidB, pidC, pidD;

	pidA = fork();

	if (pidA < 0)

	{

		fprintf(stderr, "Fork Failed");

		return (1);

	}

	else if (pidA == 0)

	{

		// Process A code

		close(fd1[0]);

		close(fd2[0]);

		close(fd2[1]);

		close(fd3[0]);

		close(fd3[1]);

		close(fd4[0]);

		close(fd4[1]);



		srand(time(0));



		while (recordNum <= 9)

		{

			int randomNum = rand() % 2;

			if (randomNum == 0)

				randomChar[0] = 'C';

			else

				randomChar[0] = 'D';



			randomChar[1] = recordNum + '0';



			write(fd1[1], randomChar, sizeof(randomChar));



			printf("A sent process B %c%d\n", randomChar[0], recordNum);



			recordNum++;

		}

	}

	else

	{

		pidB = fork();



		if (pidB < 0)

		{

			fprintf(stderr, "Fork Failed");

			return (1);

		}

		else if (pidB == 0)

		{

			// Process B code

			close(fd1[1]);

			close(fd2[0]);

			close(fd3[0]);

			close(fd3[1]);

			close(fd4[0]);

			close(fd4[1]);



			while (read(fd1[0], randomChar, sizeof(randomChar)) > 0)

			{

				if (randomChar[0] == 'C')

					write(fd2[1], randomChar, sizeof(randomChar));

				else if (randomChar[0] == 'D')

					write(fd4[1], randomChar, sizeof(randomChar));



				printf("B sent process %c %c%d\n", randomChar[0],

												randomChar[0],

												randomChar[1] - '0');

				sleep(1);

			}

		}

		else

		{

			pidC = fork();



			if (pidC < 0)

			{

				fprintf(stderr, "Fork Failed");

				return (1);

			}

			else if (pidC == 0)

			{

				// Process C code

				close(fd1[0]);

				close(fd1[1]);

				close(fd2[1]);

				close(fd3[0]);

				close(fd3[1]);

				close(fd4[0]);

				close(fd4[1]);



				while (read(fd2[0], randomChar, sizeof(randomChar)) > 0)

				{

					printf("C received %c%d\n", randomChar[0],

										randomChar[1] - '0');

				}

			}

			else

			{

				pidD = fork();



				if (pidD < 0)

				{

					fprintf(stderr, "Fork Failed");

					return (1);

				}

				else if (pidD == 0)

				{

					// Process D code

					close(fd1[0]);

					close(fd1[1]);

					close(fd2[0]);

					close(fd2[1]);

					close(fd3[0]);

					close(fd3[1]);

					close(fd4[1]);



					while (read(fd4[0], randomChar,

								sizeof(randomChar)) > 0)

					{

						printf("D received %c%d\n", randomChar[0],

										randomChar[1] - '0');



						sleep(5);

					}

				}

				else

				{

					// Logging process code

					close(fd1[0]);

					close(fd1[1]);

					close(fd2[0]);

					close(fd2[1]);

					close(fd3[0]);

					close(fd3[1]);

					close(fd4[0]);

					close(fd4[1]);



					while (logCount < MAX_NUM)

					{

						time_t rawtime;

						struct tm *timeinfo;



						time(&rawtime);

						timeinfo = localtime(&rawtime);



						printf("[%s] Log %d\n",

									asctime(timeinfo), logCount);



						logCount++;

					}



					// Terminate other processes

					kill(pidA, SIGTERM);

					kill(pidB, SIGTERM);

					kill(pidC, SIGTERM);

					kill(pidD, SIGTERM);

				}

			}

		}

	}

	return (0);

}
