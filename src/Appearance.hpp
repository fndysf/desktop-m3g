#ifndef __APPEARANCE_HPP__
#define __APPEARANCE_HPP__

#include "Object3D.hpp"
#include "m3gdef.hpp"
#include <iosfwd>
#include <vector>

namespace m3g {

    class CompositingMode;
    class Fog;
    class Material;
    class PolygonMode;
    class Texture2D;


    /**
     * set of component objects that define the rendering attributes of a Mesh or Sprite3D.
     * MeshやSprite3Dのレンダリング属性を定義するコンポーネントオブジェクトの集合.
     */

    class Appearance : public Object3D
    {
        friend class Mesh;     // for render
        friend class Sprite3D; // for render

    public:
        /** 
         * @~English  Constructs an Appearance object with default values.
         * @~Japanese Appearanceオブジェクトの作成.
         */
        Appearance ();

        /**
         * @~English  Destruct an Appearance object.
         * @~Japanese このオブジェクトを破壊するデストラクタ.
         */
        virtual ~Appearance ();

        /**
         * @~English  
         * @~Japanese このノードをアニメーションする.
         */
        virtual int animate (int world_time);

        /**
         * @~English  Creates a duplicate of this Object3D. 
         * @~Japanese このオブジェクトの複製の作成.
         */
        virtual Appearance* duplicate () const;

        /**
         * @~English  Returns the current CompostingMode for this Appearance.
         * @~Japanese このアピアランスのCompositingModeの取得.
         */
        CompositingMode* getCompositingMode () const;

        /**
         * @~English  Returns the current foggin attributes for this Appearance.
         * @~Japanese このアピアランスのFogの取得.
         */
        Fog* getFog () const;

        /**
         * @~English  Gets the current rendering layer for this Appearance.
         * @~Japanese このアピアランスのレンダリングレイヤーの取得.
         */
        int getLayer () const;

        /**
         * @~English  Gets the current rendering layer including tranceparency for this Appearance, for inner use.
         * @~Japanese このアピアランスの透明度を考慮したレンダリングレイヤーの取得。内部使用専用.
         */
        int getLayer2 () const;

        /**
         * @~English  
         * @~Japanese このアピアランスのMaterialの取得
         */
        Material* getMaterial () const;

        /**
         * @~English  Returns the current PolygonMode for this Appearance.
         * @~Japanese このアピアランスのPolygonModeの取得.
         */
        PolygonMode* getPolygonMode () const;

        /**
         * @~English  
         * @~Japanese 指定されたテクスチャーユニットのテクスチャーイメージとその属性値の取得.
         */
        Texture2D* getTexture (int index) const;

        /**
         * @~English  Sets the CompositingMode to user for this Appearance.
         * @~Japanese このアピアランスで使用するCompositingModeの設定.
         */
        void setCompositingMode (CompositingMode* compositing_mode);

        /**
         * @~English  Sets the fogging attributes to use for this Appearance.
         * @~Japanese このアピアランスで使用するFogの設定.
         */
        void setFog (Fog* fog);

        /**
         * @~English  Sets the rendering layer for this Appearance.
         * @~Japanese このアピアランスのレンダリングレイヤーの設定.
         */
        void setLayer (int layer);

        /**
         * @~English  
         * @~Japanese このアピアランスのMaterialの設定.
         */
        void setMaterial (Material* material);

        /**
         * @~English  Sets the PolygonMode to use for this Appearance.
         * @~Japanese このアピアランスのPolygonModeの設定
         */
        void setPolygonMode (PolygonMode* polygonMode);

        /**
         * @~English  Sets the texture image and its attributes for the given textureing unit.
         * @~Japanese 指定されたテクスチャーユニットのテクスチャーイメージとその属性値の設定.
         */
        void setTexture (int index, Texture2D* texture);

        /**
         * @~English  Print out information of this class, for debug only.
         * @~Japanese このAppearanceクラスの情報を表示する。デバッグ用.
         */
        virtual std::ostream& print (std::ostream& out) const;

  
        /**
         * @~English  Render this object, inner use only.
         * @~Japanese このノードをレンダリングする内部使用の関数.
         */
        virtual void render (RenderState& state) const;


        static void renderX ();

    private:
        int                     layer;
        PolygonMode*            polygon_mode;
        CompositingMode*        compositing_mode;
        Material*               material;
        std::vector<Texture2D*> textures;
        Fog*                    fog;
    };


} // namespace m3g {

#endif
