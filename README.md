# PHP Extension 開発

PHP Extensionとは、PHPの機能を拡張するためのモジュールのことです。
C言語で書かれています。（PHPを実行しているフレームワークがC言語で書かれているため）

※開発の参考用にファイルを残していますが、src内のファイルは全て削除してから、以下手順で作成しても問題ありません。

## ひな形の作成

必要なPHPバージョンに併せてコンパイルを行う
ひな形の作成には、PHPのソースファイルに存在する、ext_skelを使用する。
ext_skelの実行にもPHPの実行環境が必要となる。（開発環境を汚さないようにDockerの利用を推奨）
コンパイルにはひな形のみ必要となり、作成後はPHPのソースファイル自体は不要となる。

### Dockerの実行環境を準備

※Dockerはインストール済みの前提

1. コマンドの作成コマンドを実行
```
docker compose up -d --build
```
※当リポジトリ内に、Dockerコンテナの作成設定をしているので、そのまま実行可能
- `docker-compose.yml`
- `.dockerbuild/aml2/Dockerfile`

### ひな形の作成手順

1. Dockerコンテナのコマンドプロンプトを起動

2. 任意のパスに移動
```
cd /home/ec2-user
```

3. GitHubからPHPのソースをDL
https://github.com/php/php-src
```
git clone https://github.com/php/php-src
```

4. DLしたPHPのフォルダに移動
```
cd /home/ec2-user/php-src
```

5. ブランチを変更
```
git branch PHP-8.0
```
※`PHP-8.0`の部分は実行環境に合わせて、ブランチを変更

6. リポジトリ内のextに移動
```
cd /home/ec2-user/php-src/ext
```

7. ひな形ファイルの作成を実行
```
./ext_skel.php --ext helloworld
```
※`helloworld`の部分は好きな名称を付けてOK
※PHP実行環境が必要
※extフォルダ内に作成される`helloworld`フォルダがひな形となる

8. `helloworld`フォルダ内のひな形ファイルを、当リポジトリのsrcフォルダにコピー
```
cp -pR /home/ec2-user/php-src/ext/helloworld/* /usr/local/app/src
```

### コンパイル準備

1. `helloworld`フォルダ内のファイルを、当リポジトリの`src`フォルダにコピー

2. 当リポジトリの`src`フォルダに移動
```
cd /usr/local/app/src
```

3. コンパイル準備

```
phpize
./configure
```

4. コンパイル実行
```
make
```
上記コマンドで`src/modules`に、`helloworld.so`が作成されます。
この`helloworld.so`がPHP拡張モジュールです。

5. インストール
```
make install
```
上記コマンドで`helloworld.so`を規定のフォルダにコピーされます。

```
php -i |grep -i extension_dir
```
上記コマンドでPHP Extensionを格納する規定のフォルダを確認可能です。

6. 読み込み設定
```
cp /usr/local/app/.dockerbuild/aml2/40-helloworld.ini /etc/php.d
```
設定ファイルを設定ファイルを格納する規定のフォルダにコピー

7. nginxの再起動
```
sudo nginx -s reload
```
設定ファイルを読み込むために再起動を行います。

## 注意事項

### functionの追加に関して
`helloworld_arginfo.h`,`helloworld.c`を変更してください。
※サンプルとして`test3`を追加しています。

### コンパイル対象のファイルについて
ファイルを分ける場合は、`include`フォルダに追加して管理することを推奨します。
`config.m4``config.w32`を編集して、`include`フォルダ内のファイルを読み込むように指定するはずですが、このあたりがまだ微妙です。
分かり次第追記します。

### 参照渡しについて
値を参照渡しにする場合は、zval（型の固定されていない変数定義）固定となります。
function内でzalに、どの型を使用して値をセットするかを指定します。

```
// 参照渡し変数に、文字列型で文字列をセット
zend_try_assign_typed_ref_stringl(edit_file->value.ref, 文字列, 文字数);
```

参照渡し変数への値セット用Functionは以下の通りです。
必要なFucntionを自身で選択してください。
処理内容は、PHPのソースファイル内にある`Zend/zend_API.c`に実装されています。
```
zend_try_assign_typed_ref_ex(zend_reference *ref, zval *val, zend_bool strict)
zend_try_assign_typed_ref(zend_reference *ref, zval *val)
zend_try_assign_typed_ref_null(zend_reference *ref)
zend_try_assign_typed_ref_bool(zend_reference *ref, zend_bool val)
zend_try_assign_typed_ref_long(zend_reference *ref, zend_long lval)
zend_try_assign_typed_ref_double(zend_reference *ref, double dval)
zend_try_assign_typed_ref_empty_string(zend_reference *ref)
zend_try_assign_typed_ref_str(zend_reference *ref, zend_string *str)
zend_try_assign_typed_ref_string(zend_reference *ref, const char *string)
zend_try_assign_typed_ref_stringl(zend_reference *ref, const char *string, size_t len)
zend_try_assign_typed_ref_arr(zend_reference *ref, zend_array *arr)
zend_try_assign_typed_ref_res(zend_reference *ref, zend_resource *res)
zend_try_assign_typed_ref_zval(zend_reference *ref, zval *zv)
zend_try_assign_typed_ref_zval_ex(zend_reference *ref, zval *zv, zend_bool strict)
```
