#include "forkLabs.h"

#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

int main() {
	if (forkLabs::COUNT_PROCESSES == 0) {
		std::cerr << "Can\'t determine the number of processor cores\nExit\n";
		return -1;
	}

	int fd[2];
	int idx				= 0;
	pid_t forkResult	= 0;
	ssize_t writtenBytes = 0;
	/* Попытаемся создать pipe */
	if (pipe(fd) < 0) {
		std::cerr << "Can\'t create pipe\n";
		return -1;
	}
	/* Порождаем новые процессы */
	for (int i = 0; i < forkLabs::COUNT_PROCESSES; ++i) {
		forkResult = fork();
		if (forkResult == -1) {
			std::cerr << "Can\'t fork child number " << i << "\n";
			return -1;
		} else if (forkResult == 0) {
			/* В дочернем процессе не порождаем новые процессы */
			idx = i;
			break;
		}
	}
	/* Если в дочернем процессе */
	if (forkResult == 0) {
		/* Дескриптор для чтения не нужен */
		close(fd[0]);
		int result =
			forkLabs::countMaximums(forkLabs::xA + idx * forkLabs::COUNT_ITERS_X_PROCESS * forkLabs::xI, forkLabs::yA,
									forkLabs::COUNT_ITERS_X_PROCESS, forkLabs::count_iterations_y);
		writtenBytes = write(fd[1], &result, sizeof(result));
		if (writtenBytes != sizeof(result)) {
			/* Если записалось меньшее количество байт, сообщаем об ошибке и завершаем работу */
			std::cerr << "Can\'t write all bytes\n";
			return -1;
		}
		/* Дескриптор для записи не нужен */
		close(fd[1]);
	} else { /* Если в родительском процессе */
		/* Дескриптор для записи не нужен */
		close(fd[1]);
		/* Досчитываем остаток */
		int result = forkLabs::countMaximums(
			forkLabs::xA + forkLabs::COUNT_PROCESSES * forkLabs::COUNT_ITERS_X_PROCESS * forkLabs::xI, forkLabs::yA,
			forkLabs::count_iterations_x - forkLabs::COUNT_PROCESSES * forkLabs::COUNT_ITERS_X_PROCESS,
			forkLabs::count_iterations_y);
		for (int i = 0; i < forkLabs::COUNT_PROCESSES; ++i) {
			if (wait(nullptr) == -1) {
				std::cerr << "Error while waiting for the child processes\n";
			} else {
				ssize_t readBytes = 0;
				int temp;
				readBytes = read(fd[0], &temp, sizeof(result));
				if (readBytes < 0) {
					std::cerr << "Can\'t read data\n";
					return -1;
				}
				result += temp;
			}
		}
		/* Дескриптор для чтения не нужен */
		close(fd[0]);
		std::cout << result << "\n";
	}
	return 0;
}
