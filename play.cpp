#include <cstdint>
#include <cstdio>
#include <vector>

#include <portaudio.h>
#include <windows.h>

static unsigned char textbuf[2000];

static CHAR_INFO characters[80 * 25];

static char ansibuf[40000];

static uint32_t* audioframes;
static uint64_t audiosize = 0;
static uint32_t* abstimeframes;
static uint64_t index = 0;

void display_text()
{
    ansibuf[0] = 0;
    DWORD bw;

    //sprintf(ansibuf, "\x1b[1;1H%c", 0);

    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 80; x++) {
            //sprintf(&ansibuf[strlen(ansibuf)], "\x1b[%dm ", textbuf[y * 80 + x] > 127 ? 47 : 40);
            characters[y * 80 + x].Attributes = textbuf[y * 80 + x] > 127 ? 0xF0 : 0x00;
            characters[y * 80 + x].Char.AsciiChar = ' ';
        }
        //if (y != 24)
         //   sprintf(&ansibuf[strlen(ansibuf)], "\n");
    }
    //sprintf(&ansibuf[strlen(ansibuf)], "\x1b[0m");
    printf("%s", ansibuf);
    COORD lefttop, scrsize;
    lefttop.X = 0;
    lefttop.Y = 0;
    scrsize.X = 80;
    scrsize.Y = 25;
    SMALL_RECT rect;
    rect.Top = 0;
    rect.Bottom = 24;
    rect.Left = 0;
    rect.Right = 79;
    WriteConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE), characters, scrsize, lefttop, &rect);
    //WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), ansibuf, strlen(ansibuf), &bw, NULL);
}

int play_callback(
    const void *input, void *output,
    unsigned long frameCount,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData)
{
    uint32_t* out = (uint32_t*)output;
    for (int i = 0; i < frameCount; i++) {
        *out = audioframes[index];
        out++;
        index++;
        if (index >= audiosize) {
            return paComplete;
        }
    }
    return paContinue;
}

uint32_t search_frames(int absms, int frames, int start)
{
    for (int i = start; i < frames; i++)
    {
        if (abstimeframes[i] >= absms) {
            if (i > 0) 
                i -= 1;
            return i;
        }
    }
    return 0xFFFFFFFF;
}

int main()
{
    PaStream* strm = nullptr;
    FILE* file = fopen("./out.bin", "rb");
    FILE* file3 = fopen("./out.tim", "rb");
    FILE* file2 = fopen("./badapple.pcm", "rb");
    auto error = Pa_Initialize();
    if (error != paNoError)
        return -1;
    error = Pa_OpenDefaultStream(&strm, 0, 2, paInt16, 48000, 2352, play_callback, NULL);
    if (file && error == paNoError && file2) {
        {
            fseek(file2, 0, SEEK_END);
            auto filesize = ftell(file2);
            fseek(file2, 0, SEEK_SET);
            audiosize = filesize / 4;
            audioframes = (uint32_t*)calloc(filesize, 1);
            auto readdata = fread(audioframes, 1, filesize, file2);
            printf("read = %llu\n", readdata);
            fclose(file2);
        }
        {
            fseek(file3, 0, SEEK_END);
            auto filesize = ftell(file3);
            fseek(file3, 0, SEEK_SET);
            abstimeframes = (uint32_t*)calloc(filesize + 32, 1);
            fread(abstimeframes, 1, filesize, file3);
            fclose(file3);
        }
        fseek(file, 0, SEEK_END);
        auto filesize = ftell(file);
        fseek(file, 4000, SEEK_SET);
        auto frames = (filesize / 2000);
        timeBeginPeriod(1);
        CONSOLE_CURSOR_INFO info;
        info.bVisible = FALSE;
        info.dwSize = 1;
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
        if ((error = Pa_StartStream(strm)) != paNoError) {
            return error;
        }
        auto begTime = Pa_GetStreamTime(strm);
        int prev_frame = 0;
        while (1) {
            auto ms = (Pa_GetStreamTime(strm) - begTime) * 1000;
            prev_frame = search_frames(ms, frames, 0);
            fseek(file, 2000 * prev_frame, SEEK_SET);
            fread(textbuf, 2000, 1, file);
            display_text();
            if (prev_frame >= frames)
                break;
        }
        Pa_StopStream(strm);
        fclose(file);
    }
}