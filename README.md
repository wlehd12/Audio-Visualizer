# Audio-Visualizer

### Wave 파일 분석

![image](https://github.com/wlehd12/Audio-Visualizer/assets/125344095/6adb0f30-9b81-47af-9960-f45f46f92d89)


Chunk ID : fmt 라는 고정값이 들어감

Chunk Size : 현제 Header 뒤에 이어지는 값들의 사이즈 (24 – 8 = 16byte)

Audio Format : PCM인 경우 고정값 1을 반환

NumChannels : 음성 파일의 채널 수(mono : 1 , stereo : 2)

Sample Rate : 1초 동안 저장하는 소리 조각 수(Hz)

Byte Rate : 1초 동안 소리를 내는데 필요한 byte

BlockAlign : Sample Frame의 크기(Sample 크기 * NumChannels)

Bit Per Sample : Sample 한 개를 나타내는데 이용되는 bit수

### 헤더 설정

![그림1](https://github.com/wlehd12/Audio-Visualizer/assets/125344095/8c5a6668-6ad6-4fc0-9804-49b0f563581f)

### MCI를 이용한 소리 녹음 및 재생

#### OnKeyDown에서 Spacebar를 입력 받았을 때

HeapAlloc을 이용해 녹음 버퍼용 메모리와 재생 버퍼용 메모리를 정의해준다.

입력장치와 출력장치를 열어주고 녹음 버퍼를 모아 녹음장치를 실행시켜준다.

waveInOpen() : 사운드 입력 장치

waveOutOpen() : 사운드 출력 장치

waveInStart() : 녹음 시작

#### WindowProc에서 재생
lParam에 저장되는 음성 정보를 이용하여

녹음 버퍼의 데이터와 길이를 재생 버퍼에 복사한다

재생 버퍼를 모아 재생장치를 실행시켜준다.

waveOutWrite() :재생

#### OnTimer

녹음된 음성 주파수 p_data값과 길이를 이용해 실시간으로 파형의 크기 변화를 줄변수를 만들어 낸다.

p_data의 sample 하나당 16byte가 사용되므로 16byte 정수 저장범위의 최대값인 65535를 나누어 준다.

p_data값이 될때까지 p값이 증가 

p값이 p_data값보다 클 경우 감소 

p값의 변화에 따라 _pos.y값이 변함




