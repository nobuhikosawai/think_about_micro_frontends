= Micro Frontendsのお気持ち

== はじめに
この章ではマイクロサービスの恩恵を更に享受するために、Micro Frontendsという考え方を紹介します。
まだ新しい技術で筆者も考えを整理しきれていないのが正直なところですが、その方向性や克服すべき技術課題には大きな可能性を感じています。この章が一つの考えの種となれば幸いです。
（なお、内容や動作するコードは継続的にgithubにあげていきたいと考えています。マサカリも含めて
@<href>{https://github.com/nobuhikosawai/thinking_about_micro_frontends}に投げて頂けると幸いです）

== TL;DR

 * バックエンドがMicroservicesなのに、webフロントがモノリシックなのは甘えです。
 * Micro Frontendsも段階がある。手近なところなら今からでも初められそう
 * 本気だすならWeb Componentsに未来がありそう。ただしまだまだこれからなので、読者の方と一緒に知見を貯めていきたいです。

== Micro Frontendsとは
=== 背景
最近のマイクロサービスの普及に伴って、バックエンドにマイクロサービスアーキテクチャを導入している企業が増えてきました。導入には技術・組織双方の課題解決が必要ですが、実際の知見やAWSやDockerなどの技術的支援も増え、導入の敷居が下がって来ました。一方、フロントエンドも従来のサーバーサイドがHTMLを生成していたものよりも、よりリッチな体験をwebフロントに求められるようになってきました。フロントエンド側も技術が進歩し、バックエンドとは独立して動く、Single Page Application (SPA) の導入例が増えてきました。

=== Frontend Monolith
昨今よく見かけるSPAフロントエンドというものは、一つのフロントエンドクライアントが機能に応じて複数のバックエンドのAPIを叩き分けるというものです。フロントエンドが叩く先が少なければほとんど問題になりませんが、叩く先が増えてくると問題が生じてきます。

最も大きな問題は組織的な問題でフロントエンドを担当しているチームだけがかなり多くのビジネスドメインにまたがってしまうのです。そのためマイクロサービスが求める各サービスの自律性が失われてしまいます。バックエンドが複数に分かれているのにフロントだけがモノリシックになっている状態で、これがFrontend Monolith と呼ばれる状態です。こうした状況ではたとえバックエンドがマイクロサービスで自由にデプロイできたとしても、それをユーザーに届けるためのフロントエンドがモノリシックになっているため、フロントはバックエンド程気軽にデプロイを行うこともできません。フロントエンドだけはリリースが他の機能に与える影響を考慮せざるを得ず、ブランチ運用も複雑化し、バグの修正などにもいちいち別機能への影響を考慮しないといけなくなるでしょう。場合によってはリリースタイミングの調整なども入るかも知れませんが、それでは到底各サービスが自律して動いているとはいえないでしょう。最終的にはフロントエンドがマイクロサービスのボトルネックになってしまいます。

//image[frontend-monolith][Frontend Monolithの図 (micro-frontends.orgより)][scale=0.9]

=== Micro Frontends
そこで別の考えとして提唱されているのが、@<href>{https://www.thoughtworks.com/radar/techniques/micro-frontends, Micro Frontends}です。これは各マイクロサービスがUIも含めた機能全体を提供するというものです。

つまりこういうことです。Frontend Monolithでは、フロントエンドチームが各マイクロサービスが提供するAPIをUIに変換する作業と1つのページに集約する作業の2つを請け負っていました。Micro FrontendsのアプローチではAPIをUIに変換する作業は各サービスチームに任せ、UIの合成だけを担当するのです。

//image[micro-frontends][Micro Frontendsの図 (micro-frontends.orgより)][scale=0.9]

これによるメリットは、各サービスがそれぞれ自律的に動作できるようになるということです。例えばあるコンポーネントのA/Bテストをしたくなったらどうでしょうか？そうした問題にMicro Frontendsでは柔軟に対応することができます。またデプロイも各サービスごとに独立して行う事ができるようになるなど、マイクロサービスが本来持っているメリットをフロントエンドでも享受できるようになります。

=== 例
例えば架空のグルメサービス（ここではMSグルメ）を想像しましょう。グルメサービスの中身は下記のような感じです

 * 食レポ機能
 * ブログ機能
 * レストラン検索、予約、レコメンド機能

それぞれの機能はメニューバーから遷移可能になっています。この中で残念ながらブログ機能だけが今ひとつPVが稼げずKPIが伸びていませんでした。
ある日ブログサービスの施策で食レポ機能に、ユーザーの食事投稿傾向に応じて関連するブログ記事をレコメンドすることになりました。これは食レポチームの担当でしょうか？それともブログチームの担当でしょか？

Frontend Monolith ではそもそもフロントエンドは一つしかないので、フロントエンドチームもしくは食レポの機能をメインで触っているメンバーが担当するでしょう。

Micro Frontendsでは別の考えかたをします。ブログ機能のKPIはブログチームの持ち物なので、実装はブログチームが担当し、フロントエンドチームまたは食レポ機能チームはUXを管理するため表示箇所の提供だけを行います。こうすることで、ブログチームがKPIを上げるためにいくつか実行する施策はブログチームが単独で行うことができるようになります。

== Micro Frontendsに欲しいもの
Micro Frontendsが対処すべき課題は、分割されたチームがそれぞれUIパーツを提供する方法と、分割をユーザーに意識させないUI・UXの実現です。

必要なのは主に2つの要件です。

 1. UIコンポーネントそのものをclientに提供すること
 2. UIコンポーネント内部で起こるイベントのハンドリング

== Micro Frontends 事始め
Micro Frontendsにもいくつかのパターンがあり得ると思いますが、大きく分けると以下の2つに分けられるでしょう。

 1. ページ遷移をしてもいいパターン
 2. 1つのページに複数のコンポーネントを表示したいパターン

=== ページ遷移をしてもいいパターン
先のMSグルメの例だと、従来のメニューバーから遷移可能な状態というのは1の「ページ遷移をしてもいいパターン」でした。このパターンでは1つのサービスがそれぞれのViewを持つことが容易です。基本的に他サービスのViewは気にすることがなくてもいいでしょう。ただし、ユーザーから内部的にサービスが分断されていることを意識さないために複数のことを気にかける必要があるでしょう。

主には以下の3点です。

 1. Single Sign On (SSO)
 2. ヘッダーなど共通コンポーネントの管理
 3. 共通のUIパーツ

内部的にサービスが分断されていますが、ユーザーからはその違いが見えないようにするため、SSOのようにログイン機構を集約する機能は必要でしょう。またチームが分断していることによるUIの分断をさけるため、共通コンポーネントの管理と共通のUIパーツが必要です。共通のUIパーツはBootstrapのようなライブラリの導入やスタイルガイド、またはUIライブラリの自作などによって解決することができそうです。
ヘッダーなど共通コンポーネントの管理は難しい所で、要件にもよってことなるでしょう。主な対処方法は、一旦各サービスがコピペでのりきるか、社内ライブラリの作成を行う、またはどこかのサービスにロジックを集約させて各サービスがそれを取得するの3パターンになるでしょう。ロジックの複雑性によって対処方法が変わりそうです。

=== 1つのページに複数のコンポーネントを表示したいパターン
これはより難易度の高い、そして議論のしがいのあるMicro Frontendsのパターンです。
まさに先のMSグルメのブログチームの施策の例がそうで、食レポ機能という一つのページの中に本来の食レポとブログ機能が混じっています。

このパターンを今すぐ実現する現実的な方法はおそらくiframeでしょう。iframeでMicro Frontendsの必要要件を解決しようとすると下記のような対応になります。

 1. UIコンポーネントそのものをclientに提供すること => 各チームがHTMLを提供
 2. UIコンポーネント内部で起こるイベントのハンドリング => iframeのpostMessage

UIパーツは各チームがViewを従来のHTMLで返す方法で十分です。クリックイベントなどハンドリングだけが問題になりますが、これに対処する方法の一つとして[window.postMessage](https://developer.mozilla.org/ja/docs/Web/API/Window/postMessage)があります。iframeの内部のクリックイベントなどに対してcallbackでpostMessageを設定することによる親コンポーネントにイベントを通知する事ができます。これによりiframeで開いているコンポーネントと親（またはその他のコンポーネント）と協調して動作させることが可能になります。

//source[親コンポーネント]{
class Parent extends React.Component {
  componentDidMount() {
    window.addEventListener('message', this.doSomething, false);
  }
  componentWillUnmount() {
    window.removeEventListener('message', this.doSomething, false);
  }
  doSomething() {
    // do something
  }
}
//}

//source[子コンポーネント]{
function receiveMessage(event) {
  if (event.origin !== 'http://example.com') {
    return;
  }
　if (event.data === 'getHelloWorld') {
    event.source.postMessage(
      'hello world',
      event.origin
    );
  }
}

window.addEventListener('message', receiveMessage, false);
//}

== もっと頑張るMicro Frontends
さらに未来のWebの技術を使えばより高度にこれを実現することができます。それがWeb Componentsです。Web Componentsを使えばMicro Frontendsの必要要件をカバーすることができます。

 1. UIコンポーネントそのものをclientに提供すること => Custom Elements
 2. UIコンポーネント内部で起こるイベントのハンドリング => Custom Events

productionに投入するにはまだハードルが高いですが、これからのために注視しておく価値はありそうです。

=== UIの提供
先のMSグルメの例ではUIパーツを提供するのにCustom Elementsを使うことができるでしょう。
つまり各サービスはこれまでAPIレスポンスとしてJSONを返したいたところをDOMを返すようになるイメージです。
//source[JSON形式のレスポンス]{
[
  {
    "id": 1,
    "name": ”オススメの肉料理”,
    "place": "xxxx",
  },
]
//}
↓
//source[DOM形式のレスポンス]{
<blog-list>
  <ul>
	  <li>オススメの肉料理</li>
    ...
  </ul>
</blog-list>
//}

=== Eventのハンドリング
先のMSグルメの例だけではやや単純ですが、要素同士が協調して動く必要があるときがあります。
たとえば先のMSグルメの例でも、食レポ一覧から過去の食レポを選択したらオススメのブログ記事も連動して変わってほしいというような機能が追加されたとすると、これは複数コンポーネントの連動を考慮しないといけないでしょう。

Web ComponentsにはCustom Eventsという仕様があります。Custom Eventsを用いることでこれらの問題に対処することができます。DOMには従来からclickなどのイベントがありますが、同様の取り扱いであるコンポーネントのクリックイベントを別サービスのコンポーネントがsubscribeすることで、コンポーネント間の協調を実現することができるでしょう。

//source[FoodReport.js]{
class FoodReport extends HTMLElement {
  [...]
  connectedCallback() {
    [...]
    this.render();
    this.firstChild.addEventListener('click', this.selectReport);
  }
  selectReport() {
    // maybe talk to an api
    this.dispatchEvent(new CustomEvent('foodreport:selection:changed', {
      bubbles: true,
    }));
  }
  render() {
    this.innerHTML = `...`;
  }
  disconnectedCallback() {
    this.firstChild.removeEventListener('click', this.selectReport);
  }
//}

//source[BlogList.js]{
class BlogList extends HTMLElement {
  connectedCallback() {
    [...]
    window.addEventListener('foodreport:selection:changed', this.refresh);
  }
  refresh() {
    // fetch new data and render it
  }
  disconnectedCallback() {
    window.removeEventListener('foodreport:selection:changed', this.refresh);
  }
}
//}

=== その他考えるべきこと
先に上げた2点の必要要件以外にも、Micro Frontendsが解決すべきいくつかの要件があることに気づきます。

たとえばレンダリングの速度です。より高速なレンダリングを実現するにはSPAでも用いられているServer Side Rederingのテクニックがありますが、Web Componentsを使って以下にSSRを実現するかなどは一つ大きな課題です。これについては旧来から使われてるServer Side Includesなどの手法を用いて事前にWeb Componentsの中身をサーバー側で展開するテクニックなどが知られています。
また、レンダリングがレスポンスの遅いサービスに引きづられるという問題もあります。どれをSSRにしてどれをClient Side Reinderingに回すかや、その場合のLoding時の見せ方などはUXチームの判断するところとなるでしょう。

== 今後について
上で見てきたことはあくまでも概念的な話にとどまっていますが、これらをProductionのレベルで考えていこうとすれば、フレームワークやライブラリの選択、現行のSPAからのmigration pathなど、様々な問題が起こりえます。単純なページ遷移でさえ、UXの観点から各サービスが自由に行うことを禁止したくなるかも知れません。

上では十分に議論できませんでしたが、フロントエンドチーム（UI/UXチーム）がどこまで全体の体験を管理するのかということも技術・組織の双方の面で課題になりそうです。

Micro Frontendsも適切に使用すればパワフルですが、誤った用い方をすると複雑性ばかりが残るところはmicroservices共通の課題ですが、さらなるチーム間のスケールを目指してベストプラクティスを模索していきたいです。

== 参考文献
 * @<href>{https://micro-frontends.org/}
 * @<href>{https://www.thoughtworks.com/radar/techniques/micro-frontends}
 * @<href>{https://www.slideshare.net/teppeis/web-frontends-of-microservices}
