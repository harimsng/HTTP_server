#include <cstring>
#include <unistd.h>
#include <iostream>
#include <string.h>

#define	INITIAL_BUFFER_SIZE (100)
#define INCREMENT (1)
#define MAXIMUM_BUFFER_SIZE (65537)

#define BUFFER_SIZE (1000000)

int	main()
{
	char	buffer[BUFFER_SIZE] = {0, };
	int		fdSet[2];
	size_t	totalWriteCount = 0;
	ssize_t	writeCount;
	ssize_t readCount;

	pipe(fdSet);
	for (size_t buffer_size = INITIAL_BUFFER_SIZE; buffer_size < MAXIMUM_BUFFER_SIZE; buffer_size += INCREMENT)
	{
		writeCount = write(fdSet[1], buffer, buffer_size);
		std::cout << "writeCount =\t" << writeCount << '\n';
		if (writeCount <= 0)
			break;

		totalWriteCount += writeCount;
		readCount = read(fdSet[0], buffer, buffer_size);
		std::cout << "readCount =\t" << readCount << '\n';
	}
	std::cout << "totalWriteCount = " << totalWriteCount << '\n';
	if (writeCount < 0)
	{
		std::cout << strerror(errno) << '\n';
	}
}
