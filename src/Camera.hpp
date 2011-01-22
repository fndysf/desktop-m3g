#ifndef __M3G_CAMERA_HPP__
#define __M3G_CAMERA_HPP__

#include <iosfwd>
#include "m3g/Node.hpp"


namespace m3g {


    /**
     * @~English  A scene graph node that defines the position of the viewer in the scene and the projection from 3D to 2D.
     * @~Japanese シーン中のカメラ位置を定義するノード。3Dから2Dへの透視変換を行う.
     */
    class Camera : public Node
    {

    public:

        /**
         * @~English  Specifies a generic 4x4 projection matrix.
         * @~Japanese 4x4行列で汎用の投影を指定を表す定数.
         */
        static const int GENERIC     = 48;
        /**
         * @~English  Specifies a parallel projection matrix.
         * @~Japanese 平行投影を指定する定数.
         */
        static const int PARALLEL    = 49;
        /**
         * @~English  specifies a perspective projection matrix.
         * @~Japanese 透視投影を指定する定数.
         */
        static const int PERSPECTIVE = 50;

    public:
    
        /**
         * @~English  Constructs a new Camera node with default values.
         * @~Japanese Cameraオブジェクトを作成するコンストラクタ.
         */
        Camera ();

        /**
         * @~English  Destruct a Camera node.
         * @~Japanese Cameraオブジェクトのデストラクタ.
         */
        virtual ~Camera ();

        /**
         * @~English  Creates a duplicate of this Object3D. 
         * @~Japanese このオブジェクトの複製の作成.
         */
        virtual Camera* duplicate () const;

        /**
         * @~English  Copy this Object3D to specified Object3D, not defined by M3G. 
         * @~Japanese このオブジェクトのデータを引数で指定されたオブジェクトにコピーするM3G非標準の関数.
         */
        void copy (Camera* cam) const;


        virtual int animate (int world_time);

        /**
         * @~English  Gets the current projection matrix and type.
         * @~Japanese 透視変換パラメーターと種類の取得。
         *            paramsには4要素(fovy,aspect_ratio,near,far)がこの順番で書き込まれる.
         */
        int getProjection (float* params) const;

        /**
         * @~English  Gets the current projection matrix and type.
         * @~Japanese カレントの透視変換行列と種類の取得.
         */
        int getProjection (Transform* transform) const;

        /**
         * @~English  Sets the given x4 transformation as the current projcetion matrix.
         * @~Japanese 指定された4x4行列を透視変換行列として設定する.
         */
        void setGeneric (const Transform* transorm);

        /**
         * @~English  Constructs a parallel projection matrix and sets that as the current projcection matrix.
         * @~Japanese 平行投影行列を作成し、カレントの透視変換行列として設定する.
         */
        void setParallel (float height, float aspect_ratio, float near, float far);

        /**
         * @~English  Constructs a perspective projection matrix and sets that as the current projection matrix.
         * @~Japanese 射影投影行列を作成し、カレントの透視変換行列として設定する.
         */
        void setPerspective (float fovy, float aspect_ratio, float near, float far);

        /**
         * @~English  print out information of this calss, for debug only.
         * @~Japanese このCameraクラスの情報を表示する。デバッグ用.
         */
        virtual std::ostream& print (std::ostream& out) const;

        /**
         * @~English  Same as glut's lookat() function. this is not in M3G.
         * @~Japanese glutのlookat()関数と同じ。M3G比標準の関数.
         */
        void lookAt (float from_x, float from_y, float from_z,
                     float to_x, float to_y, float to_z, 
                     float up_x, float up_y, float up_z);

        /**
         * @~English  Render this Camear object, for inner use.
         * @~Japanese このCameraをレンダリングする内部使用の関数.
         */
        virtual void render (RenderState& state) const;

    protected:
        /**
         * @~English  
         * @~Japanese 
         */
        virtual void addAnimationTrack_xxx (AnimationTrack* animation_track, bool accepted);

    private:
        Camera (const Camera& cam);
        Camera& operator= (const Camera& cam);


    private:
        int   type;
        float fovy;
        float aspect_ratio;
        float near;
        float far;

        Transform projection;
    };


} // namespace m3g {

std::ostream& operator<< (std::ostream& out, const m3g::Camera& c);


#endif
