#include "TMesh.h"
#include "TMeshUtil.h"
#include "TMatrix.h"

namespace T3D
{
	TMesh::TMesh() :mObject(NULL), mSubmeshNumber(1), mCollisionType(COLLISION_COLLISION), mPolygonNumber(0), mVertexNumber(0), mRotate(0, 0, 0), mIsVisible(true)
	{
		
	}

	TMesh::TMesh(const TString& name, const TString& meshName) : mSubmeshNumber(1), mCollisionType(COLLISION_COLLISION), mPolygonNumber(0), mVertexNumber(0), mRotate(0, 0, 0), mIsVisible(true)
	{
		mName = name;
		mObject = LoadOgreMesh(meshName);

		if (!mObject)
		{
			TString errorInfo = "Mesh #" + meshName + " Not Found";
			throw errorInfo;
		}

		TObject4D* obj = mObject;
		while (obj)
		{
			mSubmeshNumber++;
			mPolygonNumber += obj->polygonNumber;
			mVertexNumber += obj->vertexNumber;
			obj = obj->nextObject;
		}
	}

	TMesh::~TMesh()
	{
		SafeDelete(mObject);
	}

	void TMesh::setScale(const TVector3D& scale)
	{
		mObject->scale = TVector4D(scale.x, scale.y, scale.z);
	}

	void TMesh::setPosition(const TVector3D& pos)
	{
		mObject->worldPosition = TVector4D(pos.x, pos.y, pos.z);
	}

	TVector3D TMesh::getPosition()const
	{
		TVector4D pos = mObject->worldPosition;
		return TVector3D(pos.x, pos.y, pos.z);
	}

	void TMesh::Move(const TVector3D& mov)
	{
		mObject->worldPosition = mObject->worldPosition + TVector4D(mov.x, mov.y, mov.z);
	}

	//ÈÆyÖáÐý×ª
	void TMesh::Yaw(TFloat yDegree)
	{
		mRotate.y = yDegree;
		TMatrix44 rotMat;
		GetRotateMatrix44Y(rotMat, yDegree);
		Transform_Object4D(mObject, rotMat, TRANSFORM_LOCAL_TO_TRANS);
	}

	//ÈÄzÖáÐý×ª
	void TMesh::Roll(TFloat rDegree)
	{
		mRotate.z = rDegree;
		TMatrix44 rotMat;
		GetRotateMatrix44Y(rotMat, rDegree);
		Transform_Object4D(mObject, rotMat, TRANSFORM_LOCAL_TO_TRANS);
	}

	//ÈÄxÖáÐý×ª
	void TMesh::Pitch(TFloat pDegree)
	{
		mRotate.x = pDegree;
		TMatrix44 rotMat;
		GetRotateMatrix44Y(rotMat, pDegree);
		Transform_Object4D(mObject, rotMat, TRANSFORM_LOCAL_TO_TRANS);
	}

	TMaterial* TMesh::getMaterial(TInt index)
	{
		if (index<0 || index >> mSubmeshNumber)
			return NULL;

		TObject4D* object = mObject;
		TInt temp = 0;
		while (temp < index)
		{
			temp++;
			object = object->nextObject;
		}

		if (object)
			return GetMaterial(object->materialName);

		return NULL;
	}

	void TMesh::setMaterial(TInt index, TString matName)
	{
		if (index < 0 || index >= mSubmeshNumber)
			return;

		TObject4D* object = mObject;
		TInt temp = 0;
		while (temp < index)
		{
			temp++;
			object = object->nextObject;
		}

		if (object)
			object->materialName = matName;
	}

	TBool TMesh::Intersect(const TVector3D& point)
	{
		TFloat x = Abs(mObject->maxBoundingBox.x)*mObject->scale.x / 2.0f;
		TFloat y = Abs(mObject->maxBoundingBox.y)*mObject->scale.y / 2.0f;
		TFloat z = Abs(mObject->maxBoundingBox.z)*mObject->scale.z / 2.0f;

		if (point.x<mObject->worldPosition.x - x || point.x>mObject->worldPosition.x + x)
			return false;

		if (point.y<mObject->worldPosition.y - y || point.y>mObject->worldPosition.y + y)
			return false;

		if (point.z<mObject->worldPosition.z - z || point.z>mObject->worldPosition.z + z)
			return false;

		return true;
	}

	TMesh* TMesh::Clone()
	{
		TMesh* mesh = new TMesh();
		{
			mesh->mName = mName + "_Clone";
			mesh->mPolygonNumber = mPolygonNumber;
			mesh->mRotate = mRotate;
			mesh->mSubmeshNumber = mSubmeshNumber;
			mesh->mVertexNumber = mVertexNumber;

			TObject4D* object = new TObject4D();
			{
				object->attribute = mObject->attribute;
				object->avgRadius = mObject->avgRadius;
				object->direction = mObject->direction;
				object->localList = mObject->localList;
				object->transformList = mObject->transformList;
				object->materialName = mObject->materialName;
				object->maxBoundingBox = mObject->maxBoundingBox;
				object->maxRadius = mObject->maxRadius;
				object->minBoundingBox = mObject->minBoundingBox;
				object->needCull = mObject->needCull;
				object->name = mObject->name;
				object->polygonList = mObject->polygonList;
				object->polygonNumber = mObject->polygonNumber;
				object->scale = mObject->scale;
				object->state = mObject->state;
				object->vertexNumber = mObject->vertexNumber;
				object->worldPosition = mObject->worldPosition;
				object->nextObject = NULL;
			}
			mesh->mObject = object;
			mesh->mIsVisible = true;
		}
		return mesh;
	}

}