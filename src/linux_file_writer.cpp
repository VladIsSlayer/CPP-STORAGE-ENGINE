#include <fcntl.h>
#include <unistd.h>
#include <string>

class LinuxFileWriter {
    public:
        LinuxFileWriter(const std::string& file) {
            fd_ = open(file.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
        }

        ~LinuxFileWriter() {
            if (fd_ != -1) close(fd_);
        }

        void write_line(const std::string& line) {
            if (fd_ == -1) close(fd_);

            write(fd_, line.c_str(), line.size());
            write(fd_, "\n", 1);
        }

    private:
        int fd_ = -1;

}