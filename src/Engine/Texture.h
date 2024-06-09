#pragma once
#include "base.h"
#include "structs.h"

class Texture final
{
public:
	explicit Texture( const std::string& imagePath );
  explicit Texture(const std::string& imagePath, const Color4f& color);
	explicit Texture( const std::string& text, TTF_Font *pFont, const Color4f& textColor );
	explicit Texture( const std::string& text, const std::string& fontPath, int ptSize, const Color4f& textColor );
	Texture( const Texture& other ) = delete;
	Texture& operator=( const Texture& other ) = delete;
	Texture( Texture&& other ) noexcept;
	Texture& operator=( Texture&& other ) noexcept;
	~Texture();

  // Done this way for backwards compatibility
	void Draw(const Point2f& dstBottomLeft = {}, const Rectf& srcRect = {}) const;
  void Draw(const Rectf& dstRect, const Rectf& srcRect = {}) const;
	void DrawNoColor( const Rectf& dstRect, const Rectf& srcRect = {} ) const;
  void DrawColor(const Rectf& dstRect, const Color4f& color, const Rectf& srcRect = {}) const;

  void SetColor(const Color4f& color);

	float GetWidth() const;
	float GetHeight() const;
	bool IsCreationOk( ) const;
private:
	//DATA MEMBERS
	GLuint m_Id;
	float m_Width;
	float m_Height;
	bool m_CreationOk;

	// FUNCTIONS
	void CreateFromImage( const std::string& path );
	void CreateFromString( const std::string& text, TTF_Font *pFont, const Color4f& textColor );
	void CreateFromString( const std::string& text, const std::string& fontPath, int ptSize, const Color4f& textColor );
	void CreateFromSurface( SDL_Surface *pSurface );
	SDL_Surface * STBImageLoad( const std::string& path );
	void DrawFilledRect(const Rectf& dstRect) const;

  Color4f m_Color;
  bool m_UsesColor;
};
