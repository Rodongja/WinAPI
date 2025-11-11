#pragma once

class CObject;

class CGravity
{
private:
	CObject* m_pOwner;
	float m_fGravity;

	bool	m_bGround;

public:
	void SetGround(bool _b);
	void SetGravity(float _f) { m_fGravity = _f; }

	bool GetGround() { return m_bGround; }

public:
	void finalupdate();

public:
	CGravity();
	~CGravity();

	friend class CObject;
};

