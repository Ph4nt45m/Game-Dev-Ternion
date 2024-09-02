#ifndef __TEXTURE_H_
#define __TEXTURE_H_

class Texture
{
	// Member methods: 
public:
	Texture();
	~Texture();

	bool Initialise(const char* pcFilename);

	void SetActive();

	int GetWidth() const; 
	int GetHeight() const;

protected:

private:
	Texture(const Texture& texture);
	Texture& operator = (const Texture& texture);

	// Member data: 
public:

protected:
	unsigned int m_uiTextureId; 
	int m_iWidth;
	int m_iHeight;

private:

};

#endif // __TEXTURE_H_
