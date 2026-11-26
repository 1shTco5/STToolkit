# STGraphEditor

STGraphEditor 为对话、连招、任务等业务提供公共图资产基类、创建流程和图编辑窗口。

## 当前实现

插件包含 `STGraphEditorRuntime` 和 `STGraphEditor` 两个模块。已实现图资产创建、图编辑窗口、Details 页签、右键新增示例节点及示例引脚连线。

## 基本框架

| 归属 | 类型 | 职责 |
| --- | --- | --- |
| STGraphEditorRuntime（Runtime） | 抽象资产基类 `USTGraphAsset` | 将编辑图保存在 `WITH_EDITORONLY_DATA` 中的 `EditorGraph` 字段。正式业务资产子类由各业务 Runtime 模块定义。 |
| STGraphEditorRuntime（Runtime） | `USTGraphNodeInfoBase`、`USTGraphStartNodeInfo`、`USTGraphMiddleNodeInfo`、`USTGraphEndNodeInfo` | 定义可编辑的 `CustomTitle` 和三个角色的数据类型。业务节点可重写 `GetNodeInfoClass()` 指定自己的数据子类。 |
| STGraphEditor（Editor） | 抽象工厂 `USTGraphAssetFactory` | 统一创建传入的具体资产类型，以资产为 Outer 初始化编辑图并关联业务 Schema。 |
| STGraphEditor（Editor） | 抽象资产定义 `USTGraphAssetDefinition` | 统一颜色、默认分类和双击打开行为。业务子类提供资产类型和显示名称，可按需覆盖分类。 |
| STGraphEditor（Editor） | `FSTGraphEditorApp` | 初始化资产编辑会话、激活 Graph 模式，并将画布选中的节点同步到 Details。 |
| STGraphEditor（Editor） | `FSTGraphEditorAppMode` | 定义 Graph 模式的页签布局与可用的页签 Factory。 |
| STGraphEditor（Editor） | `FSTGraphPrimaryTabFactory` | 创建 Primary Tab 的 `SGraphEditor`，显示当前资产持有的编辑图。 |
| STGraphEditor（Editor） | `FSTGraphDetailsTabFactory` | 创建右侧的原生属性面板，按资产打开方式限制编辑，并将面板交给 App 更新。 |
| STGraphEditor（Editor） | `USTGraphNodeBase`、`FSTGraphAddNodeAction` | 节点基类提供右键删除。公共动作接收具体节点类，在点击位置创建节点并标记资产待保存。 |
| STGraphEditor（Editor） | `USTGraphSchema` | 提供白色的 `Flow` 引脚、基础连线规则和统一的节点创建菜单。 |
| STGraphEditor（Editor） | `USTGraphStartNode`、`USTGraphMiddleNode`、`USTGraphEndNode` | 提供红、橙、蓝三种默认标题栏颜色、默认名称和默认 `Flow` 引脚。Middle 支持动态输出。 |
| 业务 Runtime 模块 | 具体资产，例如 `USTDialogueAsset` | 继承 `USTGraphAsset`，保存游戏运行时需要的业务数据。 |
| 业务 Editor 模块 | 具体 Factory、Asset Definition | Factory 指定具体资产类型和 Schema，提供业务创建菜单项；Asset Definition 将双击的资产交给公共 Toolkit。 |
| 业务 Editor 模块 | `UEdGraphSchema` 和 `UEdGraphNode` 的业务子类 | Schema 提供节点菜单与连线规则。节点类定义标题、引脚和可编辑属性。 |

业务 Runtime 模块和 STGraphEditor 分别依赖 STGraphEditorRuntime。业务 Editor 模块依赖自己的 Runtime 模块和 STGraphEditor。游戏运行时直接加载业务资产。以对话为例，节点坐标可只保存在编辑图中，台词和选项的跳转目标则需要保存在运行时可读取的数据中。

编辑器保存的资产仍包含 `EditorGraph`。目标关闭编辑器专用数据时，`EditorGraph` 和 `GetEditorGraph()` 都不会参与编译。Cook 过滤编辑器专用数据时会剥离该属性。业务资产需通过 Runtime 模块中定义的属性保存游戏所需的数据。

`USTGraphAsset` 将编辑图 getter、字段和编辑器友元放在同一个 `WITH_EDITORONLY_DATA` 块内，块内按需使用 `public` 和 `private`。App 通过友元声明写入生成后的 `RuntimeGraph`。

`USTGraphAsset::GetRuntimeGraph()` 提供运行时数据入口。`FSTRuntimeGraph` 保存全部 ST 节点，各节点包含角色、NodeInfo 和输出引脚的目标节点 ID。入口由业务从节点数组筛选。`FindNode()` 按节点 ID 查询，`FindOutputPinInfo()` 按输出的 `PinId` 找到对应的 NodeInfo 条目和 Payload。

运行时节点、节点角色和输出记录可在蓝图中读取。业务模块可按值返回 `FSTRuntimeNode`，蓝图拆分该结构后即可读取角色、NodeInfo 和输出 Pin ID。蓝图反射已通过 UE 5.8 编辑器构建，实际蓝图连线待验证。

在 ST 图编辑窗口执行 **Save** 时，App 从 `EditorGraph` 生成 `RuntimeGraph`，把 NodeInfo 复制到资产，保留输出的所有目标节点 ID。转换失败会显示原因并保留上次成功生成的数据。已有资产需要在 ST 图编辑窗口保存一次以填充运行时图。Save As、内容浏览器保存、Save All 和 Cook 暂未接入自动转换，Cook 使用资产中已保存的运行时图。

## 源码目录

Editor 模块按职责组织。资产定义和 Factory 在 `Public/Asset/` 与 `Private/Asset/`，Schema、节点类型与创建动作在 `Public/Graph/` 与 `Private/Graph/`，编辑窗口、模式和页签在 `Private/Editor/`。示例位于 `Private/_Examples/`。

## 最小示例

`USTExampleGraphAsset`、`USTExampleGraphAssetFactory` 和 `USTExampleGraphSchema` 位于 Editor 模块的 `Private/_Examples/` 下。具体 Factory 设置 `SupportedClass`、启用 `bCreateNew`，并通过 `GetGraphSchemaClass()` 指定 Schema。公共 Factory 创建具体资产后，以该资产作为图的 Outer，设置 Schema，再调用其默认节点初始化。

在内容浏览器中选择“新建资产 → ST Graph Editor → ST Example Graph”。Editor 模块启动时注册公共基类提供的顶层分类。示例的 Asset Definition 继承公共分类和橙色，并提供具体资产类型与显示名称。业务接入时，在自己的 Runtime 模块中定义资产子类，在 Editor 模块中提供具体 Factory 和 Asset Definition。

在内容浏览器双击示例资产，用 **Primary Tab** 查看编辑图。在空白处右键，从 **Nodes** 中选择 **Add Start Node**、**Add Middle Node** 或 **Add End Node**。示例 Schema 把对应的节点类交给公共动作创建。节点保存在当前资产的编辑图中。默认布局 `STGraphEditor_Layout_v3` 将 Primary Tab 放在左侧（75%），Details 放在右侧（25%）。两侧页签默认打开。

示例节点直接继承各自基类的 `AllocateDefaultPins()`。Start 只有 `Out`，Middle 默认有一个固定的 `In` 和一个动态的 `Out`，End 只有 `In`。三种节点都使用 `Flow` 引脚，颜色和连线校验由 `USTGraphSchema` 提供。可以连接 **Start → Middle → End**。此前保存的 `USTExampleGraphNode` 现在显示为 **Middle**，已有的 `Out` 也可以通过菜单删除。

每个 `Flow` 输出最多连接一个输入，输入可以接收多个输出。把已连接的输出接到新输入时，会替换该输出的旧连线。旧资产如果有一出多连，ST 窗口保存时会指出对应节点和输出，保留一条连线后即可保存。

右键 Middle 节点主体，选择 **Add Output Pin** 增加一个独立的 `Flow` 输出。右键某个输出引脚，选择 **Remove Output Pin**，仅移除该引脚及它的连线。Middle 可以保留零个输出，保存后重开时会沿用节点保存的引脚。

Start 继承红色，Middle 继承橙色，End 继承蓝色。节点允许删除时，右键节点主体可选择 **Delete Node**，断开相关连线并移除节点。`Config/DefaultSTGraphEditor.ini` 将旧的 `STGraphNode` 类引用重定向到 `STGraphNodeBase`。

业务 Schema 重写 `GetNodeMenuEntries()`，注册具体节点类、Start/Middle/End 角色和菜单名称，也可以提供提示文字。同一角色可以注册多个节点。基类将有效条目放到 **Nodes** 分类并跳过空类和抽象类。空白处右键显示所有有效条目，从 `Out` 拖到空白处显示 Middle 或 End，从 `In` 拖出显示 Start 或 Middle。新增节点通过 `USTGraphNodeBase::AutowireNewNode()` 找到反向的 `Flow` 引脚，再由当前 Schema 校验并建立连接。

单选 ST 节点时，Details 直接显示该节点的 `NodeInfo`。取消选择、多选或删除当前节点时，属性列表会清空。重新打开 Details 页签会恢复当前选中节点的属性。提交属性修改后，画布刷新当前节点。

在 Details 的 **Node → Custom Title** 中修改标题。清空后恢复角色的默认名称。示例 Middle 的 **Description** 显示在同一个分类下。已有示例资产加载时会把旧节点的 `Description` 和旧 `NodeInfo` 的 `CustomTitle` 迁移到示例 Middle 的新数据类型。

选中 Middle 后，可在 Details 的 **Pins → Outputs** 中展开现有输出并编辑 **Display Name**。清空后恢复默认名称。列表只允许编辑已有条目的属性，输出的增删仍通过右键菜单完成。每项通过原引脚的 `PinId` 匹配，改名更新 `PinFriendlyName`，无需重建引脚。旧资产加载时从实际输出补建缺失条目，已经删到零个输出的节点保留空列表。

每条输出可选用 `FInstancedStruct Payload` 保存自己的扩展字段。业务 Middle 节点可通过 `GetOutputPayloadStruct()` 指定类型，默认留空。同步时按 `PinId` 保留已有数据，只初始化空的 Payload。Payload 的类型由节点代码决定。Details 隐藏类型选择器，展开示例 Middle 的 **Pins → Outputs** 中任一输出可直接编辑对应的 **Note**。

**重置说明**：Details 沿用 UE 的原生重置。重置 **Index** 会重置整条输出记录，可能清空 Payload。只想恢复某个字段时，请使用该字段旁的重置按钮，例如 **Note**。

每个节点持有自己的 `NodeInfo`，新节点在放置时初始化，旧节点加载时补建缺失的实例。节点保存时会带上该实例。挂在编辑图节点下的这份实例随编辑图在 Cook 时过滤。运行时需要的数据由业务 Runtime 资产持有。

## 资产图标

Editor 模块从插件的 `Resources/Icon/FlowGraph.png` 注册 `ClassIcon.STGraphAsset`（16 × 16）和 `ClassThumbnail.STGraphAsset`（64 × 64）。示例及其他业务资产子类可沿继承链使用这两个默认样式。需要独立图标时，业务插件可为自己的具体资产类注册对应样式键。

文件缺失时继续使用 UE 默认图标和缩略图。资产类型颜色由 `USTGraphAssetDefinition` 提供。

## 首版计划

| 阶段 | 实现范围 | 验收方式 |
| --- | --- | --- |
| 1. 创建并打开具体图资产 | 已实现资产、Factory、双击入口和公共 Toolkit | 新建菜单有示例资产入口，创建后图的 Outer 和 Schema 正确。双击可打开已有画布 |
| 2. 编辑并保存图 | 已实现右键创建示例节点、输入输出引脚和连线规则 | 创建两个节点并连线，保存重开后检查位置和连线恢复 |
| 3. 完善基本操作 | 已实现节点右键删除、Middle 动态输出和单选节点属性联动 | 验证删除指定输出后其余连线保留，以及零输出和 Description 在保存重开后保留 |

## 验证记录

`Flow` 输出单连线规则和旧资产保存校验已通过 UE 5.8 编辑器构建和格式检查。双向拖线替换及输入端保留其他连线待交互验证。

运行时图查询和 ST 编辑窗口的保存转换已通过 UE 5.8 编辑器及游戏目标构建和格式检查。实际资产保存重开、Payload 保留和打包后读取待验收。

每条输出的 Payload 和示例 Note 已通过 UE 5.8 编辑器构建和格式检查。Details 展开编辑以及保存重开待交互验收。

公共节点创建菜单和 Example 注册接口已通过 UE 5.8 编辑器构建及格式检查。空白右键、双向拖线筛选和创建后自动连线待编辑器内验证。

Middle 加载时通过内部方法补齐输出信息并应用显示名，Details 提交属性时才触发 `OnNodeInfoChanged()`。公共新增节点动作会拒绝只读图。上述改动及 Primary Tab 取图调整已通过 UE 5.8 编辑器构建和格式检查，本轮交互验收已由用户确认通过。

Middle 输出属性列表已通过 UE 5.8 编辑器构建和格式检查。Details 改名、右键增删后刷新，以及旧资产保存重开待交互验证。

Details 直接绑定 NodeInfo 和示例数据迁移已通过 UE 5.8 编辑器构建及格式检查。UHT 生成的废弃字段仍以 `Description` 作为序列化名称。面板显示、标题即时刷新和现有资产保存重开待交互验证。

NodeInfo 类型与自定义标题已通过 UE 5.8 的 UHT 和 C++ 编译，两份插件 DLL 已在临时目录链接验证。运行中的编辑器占用了常规输出。Details 改名、清空回退以及现有资产重开待交互验证。

`EditorGraph` 已由 UHT 标记为编辑器专用属性，`STToolkit Win64 Development` 游戏目标构建通过。编辑器目标编译通过，两份插件 DLL 已在临时目录中链接验证。运行中的编辑器占用了常规输出。现有资产重开和实际 Cook 待验证。

`STToolkitEditor Win64 Development` 已通过 UE 5.8 编译和链接，修改的 C++ 文件已通过格式检查。独立分类的菜单显示及实际创建仍待编辑器交互验证。

类图标和缩略图的注册逻辑已通过 UE 5.8 编辑器构建。

资产双击入口、Graph 模式和页签 Factory 已通过 UE 5.8 编辑器构建。重构后的双击打开和页签恢复待编辑器验证。

节点基类和右键新增动作已通过 UE 5.8 编辑器构建。右键操作和保存重开待编辑器内验证。

示例引脚、旧节点加载补齐和连线规则已通过 UE 5.8 编辑器构建及格式检查。实际拖线和保存重开待编辑器内验证。

角色节点基类、节点右键删除和源码目录调整已通过 UE 5.8 编辑器构建及格式检查。

移除插件自己的撤销和重做逻辑后，UE 5.8 编辑器构建及格式检查通过。新增和删除节点后的保存重开待编辑器内验证。

`USTGraphSchema` 与示例 Schema 的继承接入已通过 UE 5.8 编辑器构建及格式检查。重构后的引脚颜色和拖线待编辑器内验证。

Example 的 Start、Middle 和 End 节点及其右键菜单已通过 UE 5.8 编辑器构建及格式检查。完整流程的创建、连线和保存重开待编辑器内验证。

Middle 动态输出已通过 UE 5.8 的 `STToolkitEditor Win64 Development` 构建。编辑器内待验证新增多个输出、删除指定输出后其他连线保留，以及零输出保存重开。

拖线创建菜单和自动连线已通过 UE 5.8 编辑器构建及格式检查。两个拖线方向的交互行为待编辑器内验证。

Details 页签的 AppMode 注册与双栏布局已通过 UE 5.8 编辑器构建及格式检查。默认布局待编辑器内验证。

节点选中与 Details 联动已通过 UE 5.8 编辑器构建及格式检查。选中切换、删除清空、页签重开和 Description 保存待编辑器内验证。

## 维护约定

固定的编辑器显示名称使用 `FText::FromString`。

重写虚函数时，在类内声明开头显式写 `virtual`，并保留 `override`。沿继承链向上查找，按最近一个显式声明该虚函数的基类分组。头文件用 `//~Begin 基类名 Interface` 和 `//~End 基类名 Interface` 包裹声明。源文件用 `#pragma region 基类名 Interface` 和 `#pragma endregion` 包裹实现。

Toolkit 中仅返回固定值的 getter 可直接写在头文件的接口分组内。布局由 Application Mode 管理，画布由页签 Factory 创建。

每次开发变更涉及功能、模块结构、使用方式或验收结果时，在同一次变更中更新本 README。功能落地后，将其从计划移入当前实现，并记录实际完成的验证及仍待验证的行为。
