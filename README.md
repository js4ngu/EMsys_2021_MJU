# 간단한 레트로 게임 제작
EMsys_2021_MJU 12조 유종상 김지호

## 실행 방법
  + /home/ecube/EMsys/periboard/peridriver.ko 에서 모든 드라이버를 insmod 명령어로 설치
  + sudo su && TERM=linux setterm -blank 0 -powerdown 0 -powersave off > /dev/tty0 < /dev/tty0
  + ps -ax | grep X 후 X-Window를 Kill 하는 과정 2번 수행
  + fbset -fb /dev/fb0 -xres 1024 -yres 600 -depth 32 -vyres 600 -t 31250 40 40 29 13 48 3
  + fbset -fb /dev/fb1 -xres 1024 -yres 600 -depth 32 -vyres 600 -t 31250 40 40 29 13 48 3
  + /home/ecube/EMsys/EMsys_2021_MJU/Project에서 ./ttaheuheuk.out 실행하기
  
## 게임 방법
+ 게임 시작 방법
  + HOME : 게임 시작 / Vol.DN : 게임 종료
+ 게임 진행
  + 갈색 벽에 닿지 않고 빨간색 > 초록색 > 파란색 오브를 순서대로 터치
  + 점수는 빠르게 줄어든다. 클리어 할 때 까지 멈추지 않으며, 클리어 했을 때 멈춘 숫자가 최종 점수
  + 3 스테이지에서 총 8개의 오브를 터치하면 클리어이며, 터치한 오브의 수는 LED의 수로 나타난다.
  + 공의 속도는 현재 키트의 주변 온도에 의해 결정되며, 0~12단계까지 존재한다. 난이도는 Text LCD에 나타난다.
  + 실패하거나 클리어 시 Home 버튼을 누르면 초기 화면으로 돌아가며, Vol.DN 버튼을 누르면 종료한다.

## 추후 업데이트
  + 오브의 스폰 위치를 랜덤하게 지정해 볼 예정이다.
