# KatanaZero
![Image](https://github.com/user-attachments/assets/d31d4e07-b017-4783-a54e-66265fda1716)
### 개발기간
25.04.11~25.04.25
### 개발인원
|채지운|안해영|김태경|
|:---:|:---:|:---:|
|캐릭터<br>컷씬<br>맵디자인<br>UI<br>대화시스템<br>대화 에디터|이펙트<br>적<br>맵디자인|게임 시스템<br>물리<br>충돌<br>보스<br>게임 에디터|

### 주요기능 - 채지운

![Image](https://github.com/user-attachments/assets/2e027792-7854-4b8c-b480-4fa2ff93b66f)

![Image](https://github.com/user-attachments/assets/f7d36694-2d37-4fe5-a6d5-e4718e4260dc)

1. 홈화면 디자인
2. WASD로 메뉴 선택후 스페이스바로 게임 시작

![Image](https://github.com/user-attachments/assets/74c7e23a-9f3e-46e9-9cba-b2c5af3e231a)

1. 컷씬 내에서의 player 이동

![Image](https://github.com/user-attachments/assets/1b08c789-8647-4284-af8e-1eeab0d698f0)

![Image](https://github.com/user-attachments/assets/0b551b89-d837-4656-aeff-955626ddfd4d)

![Image](https://github.com/user-attachments/assets/02534603-bc62-4f0c-840d-233caa0367a3)

![Image](https://github.com/user-attachments/assets/35ff2d55-ddea-4127-ac5d-f3e9aee785b7)

1. 말풍선과 선택지를 사용한 대화 흐름. 그에 따른 동작
2. 글자 이펙트 제작
3. WindowAPI의 BitBlt로 직접 화면 픽셀을 조작하여 화면 이펙트 제작

![Image](https://github.com/user-attachments/assets/16061811-e52a-41ac-be0f-349114930c56)

![Image](https://github.com/user-attachments/assets/13d093b6-6b03-4e6c-bd64-53c2be975444)

1. ImGUI를 통해 대화 작성 GUI 제작
2. 대화 흐름을 선으로 연결해 직관적으로 작성 가능
3. 단어가 출력될때 사용할 이펙트등을 간편하게 지정 가능
4. 작성된 일련의 대화 흐름을 JSON 파일로 저장하여 본 프로젝트에서 바로 읽어서 사용


## 캐릭터

![Image](https://github.com/user-attachments/assets/a6e807c3-12c0-4374-8cb9-d849704846d0)

1. 캐릭터의 각종 상태에 대한 애니메이션 재생
2. 공격이나 구르기등에 나타나야 할 이펙트 재생

![Image](https://github.com/user-attachments/assets/d9dbbeff-ce40-4807-a587-b3b7a92c9ec1)

1. shift키로 카타나 제로 게임의 시그니처인 시간 정지 스킬을 사용
2. 시간을 느리게 한 상태로, 무적상태가 되는 구르기 스킬을 이용해 적을 쉽게 처치할 수 있음

![Image](https://github.com/user-attachments/assets/36326486-9274-432b-998b-ece5fa18b9c6)

1. '드래곤' 캐릭터의 필살기인 순간이동 스킬
2. 시간이 정지하고 마우스로 가리키는 지점으로(거리제한 있음) 순간이동 하며, 궤적상의 모든 적을 죽임
3. 마우스쪽으로 잔상이 부드럽게 이동하고 스킬을 사용 
