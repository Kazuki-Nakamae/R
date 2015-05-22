##############################データ入力部#########################################
#ファイル名
FILENAME="/Users/kazuki/Desktop/network.txt"
#注目するタンパク名
PROTEIN="BDNF"

##############################ライブラリをロード###################################
#igraphライブラリ：主にネットワーク関連の処理に利用
library("igraph")

##############################その他設定###########################################
#フォント
par(family="HiraKakuProN-W3")
#グラフ：余白なし
par(oma = c(0, 0, 0, 0))   # 下・左・上・右の順
#グラフ:分割なし
par(mfrow=c(1,1)) 

##############################メインストリーム#####################################
#タンパク相互作用ネットワークのテキストファイルをデータテーブルへロード
(pnet=read.delim(FILENAME,header=T) )

##グラフデータ作成
#無向グラフ作成
g=graph.data.frame(pnet[1:2],directed=F)
#エッジの重みを付加
E(g)$weight <- pnet[[3]]
#ネットワーク描写
plot(g)

#次数中心性を算出
(degree=igraph::degree(g))
#自然対数値
(log_d=log(degree))
#箱ひげ図で分布を表示
boxplot(
  log_d,
  main=paste(c(PROTEIN,"周辺ネットワーク\nの次数中心性の分布"),collapse=""),
  ylab="log(Degree)",ylim=c(0,4)
)
points(log_d[PROTEIN],pch=16,col="red")
text(1.2,log_d[PROTEIN],PROTEIN,col="red")

###############################################################################


##############################グラフの調整#########################################
################手作業########################
tkp=tkplot(g)
#閉じないで次のコードに進むこと！
#Graph plot 6ならばTKP.ID=6
TKP.ID=6
##############################################
tkplot.getcoords(tkp.id = TKP.ID)
coords=tkplot.getcoords(tkp.id = TKP.ID, norm = TRUE)
#Rのコンソール画面へ反映
graph=plot(g,
           vertex.size=10, #ノードの大きさ
           vertex.shape="circle", #ノードの形
           vertex.color="white", #ノードの色
           vertex.frame.color="white", #ノードの枠の色
           edge.width=E(g)$weight*10, #エッジ属性weightをエッジの太さとする
           vertex.label.cex=0.8, #ノードラベルの文字サイズ
           vertex.label.color="red", #ノードラベルの文字色
           edge.color="gray", #エッジの色
           layout = coords,
           main=paste(PROTEIN,"周辺のタンパク相互作用ネットワーク")
)