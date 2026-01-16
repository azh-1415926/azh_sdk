#pragma once

#include <string>
#include <vector>

#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif
namespace azh::sdk::utils
{
    class command
    {
        std::string m_program_path_private;
        std::vector<std::string> m_args_private;
        std::string m_output_private;
        bool m_verbose_private;

    public:
        command(const std::string &program, bool verbose = false) : m_program_path_private(program), m_verbose_private(verbose) {}

        command &operator<<(const std::string &arg)
        {
            m_args_private.push_back(arg);
            return *this;
        }

        void run()
        {
            std::string command_str = m_program_path_private;

            for (auto &arg : m_args_private)
            {
                command_str += " " + arg;
            }

            char buffer[128];
            m_output_private.clear();
#ifdef _WIN32
            std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(command_str.c_str(), "r"), _pclose);
#else
            std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command_str.c_str(), "r"), pclose);
#endif
            if (!pipe)
            {
                throw std::runtime_error("popen() failed!");
            }
            while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr)
            {
                m_output_private += buffer;
                if (m_verbose_private)
                {
                    std::cout << buffer;
                }
            }
        }

        void run(const std::string &input)
        {
            std::string command_str = m_program_path_private;

            for (auto &arg : m_args_private)
            {
                command_str += " " + arg;
            }

#ifdef _WIN32
            m_output_private = _execute_command_win32(command_str, input);

#else
            m_output_private = _execute_command_linux(command_str, input);
#endif
            if (m_verbose_private)
            {
                std::cout << m_output_private;
            }
        }

        void clean() { m_args_private.clear(); }

        std::string output() { return m_output_private; }

    private:
#ifdef _WIN32
        inline std::string _execute_command_win32(const std::string &cmd, const std::string &input)
        {
            HANDLE hStdInRead, hStdInWrite, hStdOutRead, hStdOutWrite;
            SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};

            /* create pipe */
            if (!CreatePipe(&hStdInRead, &hStdInWrite, &sa, 0) ||
                !CreatePipe(&hStdOutRead, &hStdOutWrite, &sa, 0))
            {
                return "ERROR : CreatePipe";
            }

            /* set process info */
            STARTUPINFO si = {sizeof(STARTUPINFO)};
            si.dwFlags = STARTF_USESTDHANDLES;
            si.hStdInput = hStdInRead;
            si.hStdOutput = hStdOutWrite;
            si.hStdError = hStdOutWrite;

            PROCESS_INFORMATION pi;

            /* create process */
            if (!CreateProcess(NULL, (LPSTR)cmd.c_str(), NULL, NULL, TRUE,
                               CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
            {
                CloseHandle(hStdInRead);
                CloseHandle(hStdInWrite);
                CloseHandle(hStdOutRead);
                CloseHandle(hStdOutWrite);
                return "ERROR in CreateProcess";
            }

            CloseHandle(hStdInRead);
            CloseHandle(hStdOutWrite);

            /* write input */
            DWORD written;
            WriteFile(hStdInWrite, input.c_str(), input.length(), &written, NULL);
            CloseHandle(hStdInWrite);

            /* read output */
            std::string result;
            char buffer[4096];
            DWORD read;

            while (ReadFile(hStdOutRead, buffer, sizeof(buffer) - 1, &read, NULL) && read > 0)
            {
                buffer[read] = '\0';
                result += buffer;
            }

            /* clean */
            CloseHandle(hStdOutRead);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);

            return result;
        }
#else
        inline std::string _execute_command_linux(const std::string &cmd, const std::string &input)
        {
            int stdin_pipe[2];
            int stdout_pipe[2];
            pid_t pid;
            std::string m_output_private;

            if (pipe(stdin_pipe) < 0 || pipe(stdout_pipe) < 0)
            {
                perror("pipe");
                return "";
            }

            pid = fork();
            if (pid < 0)
            {
                perror("fork");
                return "";
            }

            if (pid == 0)
            {
                /* sub process */
                close(stdin_pipe[1]);
                close(stdout_pipe[0]);

                /* dup to stdin */
                dup2(stdin_pipe[0], STDIN_FILENO);
                /* dup to stdout */
                dup2(stdout_pipe[1], STDOUT_FILENO);

                close(stdin_pipe[0]);
                close(stdout_pipe[1]);

                /* run command */
                execl("/bin/sh", "sh", "-c", cmd.c_str(), (char *)NULL);
                perror("execl");
                exit(1);
            }
            else
            {
                /* main process */
                close(stdin_pipe[0]);
                close(stdout_pipe[1]);

                /* write input */
                write(stdin_pipe[1], input.c_str(), input.size());
                close(stdin_pipe[1]);

                /* read output */
                char buffer[256];
                ssize_t n;
                while ((n = read(stdout_pipe[0], buffer, sizeof(buffer) - 1)) > 0)
                {
                    buffer[n] = '\0';
                    m_output_private += buffer;
                }

                close(stdout_pipe[0]);
                waitpid(pid, NULL, 0);
            }

            return m_output_private;
        }
#endif
    };
}