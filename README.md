# concurrent-stock-server

## project 1
### Event-driven Approach 를 통한 concurrent-stock-server 구현

* Event기반의 처리 방식으로, 하나의 process로 여러개의 client를 핸들링한다.
Client들이 요청한 connfd들이 배열로 저장되며 select을 대기한다. Select되면 해당 이벤트를 처리한다.
이를 통해 client들이 보낸 연결 요청에 대해 서버가 connfd 배열을 순회하며 concurrent programming이 가능하다.


## project 2
### Thread-based Approach 를 통한 concurrent-stock-server 구현

* Thread 기반의 처리로, client가 연결을 요청할 때마다 새로운 thread를 생성하고 그 해당 thread의 요청을 처리한다. 각 thread들은 각자의 처리 flow을 가지고 있으면서 data, kernel context등을 공유한다. Single core processer 해서 Thread는 실제로 sequential 하기 처리되지만 time slicing을 통해 concurrent하게 돌아가는 것처럼 느껴진다.

* 쓰레드들은 함께 공유하고있는 자원들을 동기화 하기 위해 mutex를 사용한다. Mutex는 공유자원 공간에 대한 접근 시간 제어를 통해 동기화를 달성한다.쓰레드는 잠금을 얻어야 임계영역에 들어갈 수 있으며 잠금(mutex)을 얻지 못한 쓰레드는 기다린다. 일반적으로 변수의 값을 읽어와서 어떠한 연산을 거쳐 변수의 값이 변경 되는 코드 영역을 임계 영역으로 지정할 수 있다.
	P연산을 통해 락을 걸고 V연산을 통해 락을 해제한다. 오직 락을 건(mutex)를 가진 쓰레드만이 마찬가지로 mutex를 해제할 수 있다.

