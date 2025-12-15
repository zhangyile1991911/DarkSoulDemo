このGitリポジトリは、就職活動用のポートフォリオです。
主に、ダークソウルズ風の戦闘システムとクライミングシステムのデモが含まれています。

## 実装した機能

<img src=".github\graphics\blockkill.gif" width="100%">

<img src=".github\graphics\valutingkill.gif" width="100%">

## 敌人对峙时候移动逻辑（解决）
1. 先看下之前逻辑
2. 对手朝向 ,判断距离是否近或远
3.  根据夹角 左右移动
GetActorLocation()拿到的是角色Capsule中间位置坐标 应该要减去 GetScaledCapsuleHalfHeight

## 两个动画融合（解决）
1. 击退动画 + 晕眩动画
2. 击退动画有根运动 晕眩动画也有根运动
3. 在晕眩动画中通过Add Bone Transform to Additive Layer Tracks添加Translation偏移，在晕眩动画第一帧的地方添加击退动画的Y轴偏移量
4. 在ABP中使用Two Way Blend节点 融合这两个动画
5. 勾选Interp Result选项 调整Interp Speed Increasing数值

## 无法调整Blueprint的自定义数值问题（解决）
- 在CPP中 继承ActorComponent创建一个自定义CustomActorComponent
```cpp
class CustomAcctorComponent : AcctorComponent
{
public:
    UPROPERTY(BlueprintReadWrite)
    float fCustomData = 5.0f;
} 
```
- 在CPP中 继承Actor创建一个自定义的CustomActor
```cpp
CustomActor::CustomActor(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	BaseCollision = ObjectInitializer.CreateDefaultSubobject<UWeaponCollision>(this,TEXT("Collision"),false);
}
```
- 创建一个BP_CustomActor蓝图 继承自CustomActor,在BP_CustomActor中调整CustomActorComponent的fCustomData值。
- 运行时候fCustomData还是默认值5.0f
- 问题原因在
```cpp
CustomActor::CustomActor(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
    //非常重要的一个坑，最后一个参数是bTransient是临时的，所以不会被正确序列化 保存到.uasset中
	BaseCollision = ObjectInitializer.CreateDefaultSubobject<UWeaponCollision>(this,TEXT("Collision"),false);
}
```

## 添加ObjectType（解决）

在角色翻滚过程中，不被攻击。新增了一个ObjectType Rolling。

在角色翻滚时候将角色的ObjectType切换成Rolling，翻滚结束再切换回Pawn。通过这样的方法来创建无敌帧
