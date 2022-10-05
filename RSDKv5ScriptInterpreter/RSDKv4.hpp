class RSDKv4Object : public RSDK::GameObject::Entity
{
public:
	class Static : public RSDK::GameObject::Static
	{
	};

    int32 values[48];
    int32 state;
    int32 animationTimer;
    int32 animationSpeed;
    int32 lookPosX;
    int32 lookPosY;
    uint8 propertyValue;
    uint8 priority;
    uint8 animation;
    uint8 prevAnimation;
    uint8 frame;
    int8 controlMode;
    uint8 controlLock;
    uint8 pushing;
    uint8 objectInteractions;
    uint8 gravity;
    uint8 left;
    uint8 right;
    uint8 up;
    uint8 down;
    uint8 jumpPress;
    uint8 jumpHold;
    uint8 scrollTracking;
    uint8 floorSensors[5];

	RSDK_DECLARE(RSDKv4Object);
};

enum ObjectPriority {
    // The entity is active if the entity is on screen or within 128 pixels of the screen borders on any axis
    PRIORITY_BOUNDS,
    // The entity is always active, unless the stage state is PAUSED/FROZEN
    PRIORITY_ACTIVE,
    // Same as PRIORITY_ACTIVE, the entity even runs when the stage state is PAUSED/FROZEN
    PRIORITY_ALWAYS,
    // Same as PRIORITY_BOUNDS, however it only does checks on the x-axis, so when in bounds on the x-axis, the y position doesn't matter
    PRIORITY_XBOUNDS,
    // Same as PRIORITY_XBOUNDS, however the entity's type will be set to BLANK OBJECT when it becomes inactive
    PRIORITY_XBOUNDS_DESTROY,
    // Never Active.
    PRIORITY_INACTIVE,
    // Same as PRIORITY_BOUNDS, but uses the smaller bounds (32px off screen rather than the normal 128)
    PRIORITY_BOUNDS_SMALL,
    // Same as PRIORITY_ACTIVE, but uses the smaller bounds in object.outOfBounds
    PRIORITY_ACTIVE_SMALL
};
