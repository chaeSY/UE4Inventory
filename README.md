## InventoryDemo (UE 4.25)
데이터 테이블, 인벤토리, 상점 시스템 구현

<img src="https://github.com/chaeSY/InventoryDemo/blob/main/Image/MainImage.PNG?raw=true" width="100%" height="100%" title=""></img><br/>

### 구현된 기능
* 데이터 구조 <br/>
  아이템, 상점데이터<br/>

* 인벤토리<br/>
  인벤토리 탭 분류 <br/>
  아이템 추가, 삭제, 이동 <br/>
  아이템 Stack <br/>

* 상점<br/>
  테이블의 아이템 목록으로 상점 구성 <br/> 
  아이템 구매 <br/>

* UI <br/>
  Drag & Drop <br/>
  마우스 클릭 <br/>

### 동작
* 인벤토리 키: I <br/>
  인벤토리의 우측하단 +버튼을 누르면 500원을 받습니다. <br/>
  인벤토리 아이템을 우클릭하면 1개를 제거합니다. <br/>
  드래그 앤 드롭으로 아이템을 이동할 수 있습니다. <br/>

* 상점 키: O <br/>
  상점의 하단에 StoreID를 입력하고 v버튼을 누르면 StoreTable의 행번호에 대응되는 아이템 리스트를 불러옵니다. <br/>
  상점의 아이템을 인벤토리로 드래그 앤 드롭하면 구매를 시도합니다. <br/>
  상점의 아이템을 우클릭하면 구매를 시도합니다. <br/>


### 구현 설명

* UI/Widget<br/>
  데이터를 처리하는 클래스와 화면에 보여지는 클래스를 분리하는 것을 중점적으로 구현하였습니다.<br/>
  데이터에 관련된 기능을 처리하는 클래스는 USYUIBase기반의 클래스이고, 보여지는 것과 관련된 클래스는 USYWidgetBase기반의 클래스입니다.<br/>
  아래는 UI와 Widget간의 관계를 보여주는 UML입니다.<br/>
<img src="https://github.com/chaeSY/InventoryDemo/blob/main/Image/UI,Widget.PNG?raw=true" width="100%" height="100%" title=""></img><br/>
ㆍ UI는 USYUIBase기반의 클래스, USYWidetBase기반의 클래스 한쌍으로 이루어집니다. <br/>
ㆍ UI를 갱신해야 할 때 USYUIBase는 자신의 위젯에 화면 갱신을 요청합니다. <br/>
ㆍ 위젯클래스는 각 종 이벤트의 델리게이트를 정의하고, UI클래스는 필요한 기능을 바인딩합니다. <br/>

* UI/UIManager<br/>
<img src="https://github.com/chaeSY/InventoryDemo/blob/main/Image/UI,UIManager.PNG?raw=true" width="80%" height="80%" title=""></img><br/>
ㆍUSYUIBase기반의 클래스는 데이터 보관/처리를 담당합니다. <br/>
  이를테면 USYUIInventory에는 아이템 추가 기능이 있는데 빈 슬롯이 있는지, 중첩할 수 있는 아이템인지와 같은 여부를 검사합니다.<br/>
ㆍUIManager는 각각의 UI클래스에 enum값을 두어 관리하고, 블루프린트에서 USYUIBase 기반의 클래스를 생성합니다.<br/>

* Widget/LayoutWidget<br/>
<img src="https://github.com/chaeSY/InventoryDemo/blob/main/Image/Widget,LayoutWidget.PNG?raw=true" width="100%" height="100%" title=""></img><br/>
ㆍ ULayoutWidget은 USYWidgetBase기반의 위젯을 생성하고 최초 위치를 지정합니다.<br/>
ㆍ USYWidgetBase기반의 클래스는 화면에 보여질 위젯들의 클래스입니다.<br/>
  데이터를 보관하지 않는 것을 원칙으로 하며, 화면을 갱신해야 할 때는 USYUIBase기반의 클래스에게 갱신에 필요한 데이터를 전달받아 갱신합니다.<br/>
ㆍ USYInteractionWidgetBase기반의 클래스는 클릭/마우스 드래그 앤 드롭과 같은 기능이 있는 클래스들입니다. USYWidgetBase기반의 클래스에 포함시켜 기능을 사용할 수 있습니다.<br/>
  이를테면 인벤토리 위젯의 상단 이름바에 WP_InteractionMovable을 배치하였는데, 상단 바를 드래그 앤 드롭하면 인벤토리 위젯의 위치가 이동하는 기능이 추가됩니다.<br/>

* Table/GameDataManager<br/>
<img src="https://github.com/chaeSY/InventoryDemo/blob/main/Image/Table,GameDataManager.PNG?raw=true" width="100%" height="100%" title=""></img><br/>
ㆍ GameDataManager는 각각의 UDataTable에 enum값을 두어 관리하고, 블루프린트에서 생성합니다.<br/>
ㆍ UDataTable의 RowID는 숫자(ClassID)로 구분하였고, 하나의 행 정보는 ClassID와 Table의 enum Type으로 얻을 수 있습니다.<br/>