using System;

namespace Loopie
{
	public class Transform : Component
	{
		public Vector3 GetPosition()
		{
			Vector3 entityPosition = Vector3.Zero;
			InternalCalls.Transform_GetPosition(entity.ID, entityPosition);
			return entityPosition;
		}

		public void SetPosition(Vector3 position)
		{
			InternalCalls.Transform_SetPosition(entity.ID, position);
		}
	}
}
