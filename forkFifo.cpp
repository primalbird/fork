#include "forkLabs.h"

#include <iostream>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	if (forkLabs::COUNT_PROCESSES == 0) {
		std::cerr << "Can\'t determine the number of processor cores\nExit\n";
		return -1;
	}

	const char name[] = "temp.fifo";
	int fd;
	int idx				= 0;
	pid_t forkResult	= 0;
	size_t writtenBytes = 0;
	/* Обнуляем маску создания файлов текущего процесса для того,
	чтобы права доступа у создаваемого FIFO точно соответствовали
	параметру вызова mkfifo() */
	umask(0);

	/* Попытаемся создать FIFO */
	if (mkfifo(name, 0666) < 0) {
		if (errno != EEXIST) {
			std::cerr << "Can\'t create fifo\n";
			return -1;
		}
	}
	/* Порождаем новые процессы */
	for (int i = 0; i < forkLabs::COUNT_PROCESSES; ++i) {
		forkResult = fork();
		if (forkResult == -1) {
			std::cerr << "Can\'t fork child number " << i << "\n";
			return 0;
		} else if (forkResult == 0) {
			/* В дочернем процессе не порождаем новые процессы */
			idx = i;
			break;
		}
	}
	/* Если в дочернем процессе */
	if (forkResult == 0) {
		int result =
			forkLabs::countMaximums(forkLabs::xA + idx * forkLabs::COUNT_ITERS_X_PROCESS * forkLabs::xI, forkLabs::yA,
									forkLabs::COUNT_ITERS_X_PROCESS, forkLabs::count_iterations_y);
		/* Пытаемся открыть FIFO на запись */
		if ((fd = open(name, O_WRONLY)) < 0) {
			std::cerr << "Can\'t open FIFO for writing\n";
			return -1;
		}
		writtenBytes = write(fd, &result, sizeof(result));
		if (writtenBytes != sizeof(result)) {
			/* Если записалось меньшее количество байт, сообщаем об ошибке и завершаем работу */
			printf("Can\'t write all bytes\n");
			return -1;
		}
		close(fd);
	} else { /* Если в родительском процессе */
		/* Досчитываем остаток */
		int result = forkLabs::countMaximums(
			forkLabs::xA + forkLabs::COUNT_PROCESSES * forkLabs::COUNT_ITERS_X_PROCESS * forkLabs::xI, forkLabs::yA,
			forkLabs::count_iterations_x - forkLabs::COUNT_PROCESSES * forkLabs::COUNT_ITERS_X_PROCESS,
			forkLabs::count_iterations_y);
		/* Пытаемся открыть FIFO на чтение */
		if ((fd = open(name, O_RDONLY)) < 0) {
			std::cerr << "Can\'t open FIFO for reading\n";
			return -1;
		}
		for (int i = 0; i < forkLabs::COUNT_PROCESSES; ++i) {
			if (wait(nullptr) == -1) {
				std::cerr << "Error while waiting for the child processes\n";
			} else {
				size_t readBytes = 0;
				int temp;
				readBytes = read(fd, &temp, sizeof(result));
				if (readBytes < 0) {
					std::cerr << "Can\'t read data\n";
					return -1;
				}
				result += temp;
			}
		}
		close(fd);
		std::cout << result << "\n";
	}
	return 0;
}
