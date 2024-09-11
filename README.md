## 概要
FiveM/GTA Online向けの外部チートです。チート開発に対する興味を失ったので公開しておきます。  
作りかけですが、コードを書けないガキが売ってるクソみたいなチートよりはマトモです。研究、分析、学習や調査用にどうぞ。

## 機能
* AIMBOT
* ESP
* PlayerList
* Local Mod
* MenuKey : INSERT

## 備考
* エイム予測はあまり良くないです。無いよりはかなりマシですが、より完璧なものにする場合は少し工夫が必要です。
* プレイヤーの名前を取得するコードを含むいくらかのコードが削除されています。
* メニューにImGuiを採用、ESP等のレンダリングもImGuiのDrawListを使用。
* ImGuiに最低限のカスタムを施しているので参考にすることも可。（ImGui/Custom.cpp）
* ある程度フレームワーク化を行ったので、他のゲームのチートへ比較的簡単に転用可能。

## 実行
必要なライブラリは全て揃っているので、VisualStudioでビルドすれば普通に動くはずです。  
https://learn.microsoft.com/ja-jp/cpp/build/vscpp-step-0-installation?view=msvc-170

## 免責事項
このプロジェクトは学習や研究・教育用としてアップロードされました。  
これらの用途以外で使用した場合に発生した如何なる損害についても、製作者(Neko64V)は一切の責任を負いません。  

## 使用したライブラリ
* Microsoft DirectXTK->SimpleMath  
https://github.com/microsoft/DirectXTK  
* ImGui  
https://github.com/ocornut/imgui  
* FreeType  
https://github.com/freetype/freetype

## スクリーンショット
![image](https://github.com/user-attachments/assets/f4660917-e376-4d32-8d03-146d6cf7e048)
![image](https://github.com/user-attachments/assets/43d642a7-5cea-45da-aeaa-fca1dd4422fc)
