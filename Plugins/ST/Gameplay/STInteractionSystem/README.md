# STInteractionSystem

可配置的 Unreal Engine 交互系统。当前支持本地玩家发现目标、筛选候选、选择焦点和运行候选表现 Action。

## 接入与职责

- 在发起方挂载 `USTInteractorComponent`。公开入口为 `RequestInteractionWithFocus`、`RequestInteraction(Target)`。候选集合 `RawTargets`、`ValidTargets` 与焦点 `FocusedTarget` 供蓝图只读。
- 新建交互物蓝图时选 `ASTInteractableActor` 为父类。选中 `InteractableComponent` 配置规则与动作，选中 `InteractionPoint` 调整点位的相对位置和朝向。
- `InteractionPoint` 是挂在 Actor 根节点下的原生 `USceneComponent`。`USTInteractableComponent::GetInteractionPoint()` 直接读取该字段。使用其他父类的 Actor 蓝图可以添加名为 `InteractionPoint` 的 Scene Component。缺少点位时返回空。
- 在 Actor 蓝图中实现接口函数 `CanInteract` 并返回布尔值，在事件图实现 `OnInteractionCompleted`。已有其他父类的 Actor 蓝图可以在 Class Settings 中添加 `STInteractableInterface`，挂载一个 `STInteractableComponent`，并在 Actor 蓝图中实现同一套回调。上下文 `FSTInteractionContext` 携带发起方、目标方各自的组件和 Actor。
- C++ 使用方在自己的模块依赖中加入 `STInteractionSystem`。公开头文件位于 `Source/STInteractionSystem/Public/`。
- 插件源码文件和自定义目录使用单数命名，例如 `STInteractableActor.h`。

## 模块化配置

### 交互前

检测由玩家侧的 `USTInteractorComponent::TickComponent` 驱动。取得本地玩家控制器后，Tick 依次调用 `QueryRawTargets` 查询范围、`FilterTargets` 更新有效候选、`SelectFocusedTarget` 选出焦点、`RefreshPresentations` 刷新表现。失去本地控制或 EndPlay 时通过 `ClearCandidates` 清理状态和表现。

1. 将 `USTInteractorComponent` 挂在由本地玩家控制的 Pawn 上。在 **Interaction → Query** 设置 `QueryRadius`，默认 300 cm，设为 0 时暂停发现目标。`QueryChannel` 默认使用 STToolkit 配置的 `InteractionQuery`（`GameTraceChannel1`）。在其他项目中使用插件时，可以在项目碰撞设置中创建默认响应为 Ignore 的 Trace Channel，并在组件中选中它。
2. 玩家每帧以 Pawn 的位置为球心做一次简单碰撞查询。将目标 Mesh 或专用 Box/Sphere 的查询碰撞打开，并令它对 `InteractionQuery` 响应 Overlap。查询忽略玩家自身，按命中的 Actor 上的 `USTInteractableComponent` 去重得到 `RawTargets`。旧配置中仅添加了 `Interaction` 标签的碰撞体需要补上通道响应。主动查询无需开启 Generate Overlap Events。
3. 对每个候选先查询 Actor 接口的 `CanInteract(Context)`，再依次调用配置的 `Filters`，第一个拒绝终止该目标的筛选。未实现接口的 Actor 默认允许，空 Filter 条目跳过。两类查询每帧执行，适合只读判断。
4. 通过筛选的目标进入 `ValidTargets`。角色 Actor 的前向与「角色眼部指向交互点」方向点积最大的目标成为 `FocusedTarget`。没有交互点时使用 Actor 位置。得分相同优先保持原焦点。视角限制和遮挡判断可由 Filter 实现。
5. 每个目标只运行当前状态的一组表现：未通过筛选用 Raw，已通过但未聚焦用 Valid，聚焦用 Focused。状态变化时，旧 Action 收到 Stop、Shutdown，随后发起方复制新状态的模板并调用 Initialize、Start。保持同一状态会复用当前实例。离开范围、失去本地控制或发起方结束运行时会清理表现。

候选表现仅在本地玩家一侧计算，客户端跳过 ServerOnly 动作。蓝图可从回调的 Context 取得双方 Actor 和组件。候选状态的生命周期由框架驱动。

### 配置内置蓝图

在 Content Browser 打开 **Show Plugin Content**，然后在 `STInteractionSystem Content` 下查找以下资产。

| 资产 | 配置位置 | 行为 |
| --- | --- | --- |
| `Filter/BP_InteractionViewFilter` | `Filters` 的第一项 | 用角色 Actor 的前向与眼部指向目标 Actor 原点的方向计算夹角。`HalfAngleDegrees` 默认 45°。 |
| `Filter/BP_InteractionOcclusionFilter` | `Filters` 的第二项 | 沿玩家眼部到目标 Actor 原点发出一条 Visibility 射线，忽略玩家和目标 Actor。有阻挡时从有效候选中排除。 |
| `Action/BP_InteractionMarkerAction` | `FocusedCandidateActions` 或 `ValidCandidateActions` | Start 通过 `CreateInteractionMarker` 创建标记、传入交互目标并添加到玩家屏幕。Stop 时移除标记。 |
| `UI/WBP_InteractionMarkerDefault` | 由标记 Action 创建 | 在 Construct 时将收到的 `MarkerText` 写入 `TextBlock`。目标跟随和屏幕投影由 C++ Widget 基类处理。 |

只给 `FocusedCandidateActions` 添加标记时，只有当前焦点显示标记。两组都可使用 `BP_InteractionMarkerAction`，在各自实例的 **Setting → Marker Text** 中填写要显示的文字。`BP_STInteractable` 示例将有效候选设为 `○`，焦点候选设为 `●`。遮挡物应 Block Visibility，范围碰撞体可设为 Ignore Visibility，同时保留用于候选发现的查询通道响应。两个示例 Filter 使用 Actor 原点，焦点评分优先使用 `InteractionPoint` 点位，摆放时请留意两者的位置。

| 位置 | 配置项 | 预期职责 |
| --- | --- | --- |
| Filter/ | Filters | 候选筛选。派生类重写 PassesFilter。 |
| Validator/ | Validators | 服务端请求校验。派生类重写 Validate，返回是否允许及失败原因。首版不内置服务端范围复查，可按需配置。 |
| Action/ | RawCandidateActions、ValidCandidateActions、FocusedCandidateActions | 配置三种候选状态的动作模板。 |
| Action/ | DuringInteractionStages | 每个阶段包含一组动作模板。ExecutionPolicy 描述执行端，bWaitForCompletion 用于阶段内的等待配置。 |
| Action/ | PostInteractionActions | 配置成功后的动作模板。 |

Filter、Validator 和 Action 均为抽象、可内联实例化的 UObject。创建 C++ 或蓝图派生类后，可在目标组件的对应数组中选择并配置。Action 提供 ReceiveInitialize、ReceiveStart、ReceiveStop、ReceiveShutdown，以及 Complete、Fail 两个报告入口。

## 当前行为与验证范围

`USTInteractionMarkerWidget` 是 C++ 标记基类。选择派生自它的 `MarkerWidgetClass` 后，`CreateInteractionMarker` 会在 `AddToPlayerScreen` 前调用 `SetTargetActor(Context.InteractableActor)`。目标以弱引用保存。`WorldOffset` 仍可通过创建节点传入，默认沿世界 Z 轴向上 60 cm。基类每帧投影目标当前位置，目标失效或投影失败时隐藏标记。

`MarkerText` 是基类的 `FText` 创建引脚，由标记 Action 传入。各 Widget 子类可以自行决定如何使用这段文字和如何设置字形样式。

RequestInteractionWithFocus 将有效焦点传给 RequestInteraction。请求提交、服务端 Validator、During/Post 调度及 Complete/Fail 等待后续实现。ASTInteractableActor 的默认业务资格查询返回 true；未重写的 Filter 和 Validator 均返回拒绝。

后续随实现同步维护本文件中的接入步骤、字段语义、运行行为与验证记录。

交互前逻辑已通过 UE 5.8 的 STToolkitEditor / Win64 / Development 单模块构建。首次自动化场景中的焦点断言未通过，原因尚未确认。实际交互效果由项目作者在编辑器中验收。
