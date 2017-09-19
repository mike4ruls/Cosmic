#pragma once
class Material {

public:
	Material();
	~Material();
	enum MatType {
		Opaque,
		Transulcent
	}materialType;
	unsigned int translucentID;

private:

};
