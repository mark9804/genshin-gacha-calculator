# genshin-gacha-calculator

对应原神2.0及以上版本

一个计算自己离抽到自己期望的结果时还要~~花多少钱~~攒多少抽的计算器。

由 [孤胆飞客](https://bbs.nga.cn/nuke.php?func=ucp&uid=43116386)的[C++及Excel版（对应1.6及以下版本）](https://bbs.nga.cn/read.php?tid=26447862)魔改而来。

<details>
<summary>抽卡模型</summary>

## 抽卡模型（来自： [一棵平衡树](https://space.bilibili.com/6165300)）

### 基础模型

#### 轮盘选择法

轮盘选择法是经常应用于随机选择权重不同的多类物品的算法。类似于超市中常见的大转盘抽奖，转动转盘，转盘停下的位置是随机的，因此指针指向某类物品的概率正比于某类物品所占面积。

![img](https://i0.hdslb.com/bfs/article/b489443c990bb7270bd0586affd3cce60961e67c.png@942w_609h_progressive.webp)

计算机中的实现也很容易，给定物品 ![A_1%2CA_2%2C%20%5Cdots%20A_n](https://api.bilibili.com/x/web-frontend/mathjax/tex?formula=A_1%2CA_2%2C%20%5Cdots%20A_n) ，各自权重为 ![W_1%2CW_2%2C%20%5Cdots%20W_n](https://api.bilibili.com/x/web-frontend/mathjax/tex?formula=W_1%2CW_2%2C%20%5Cdots%20W_n) 。随机一个在 ![%5B1%2C%5Csum_%5Cnolimits%7Bi%3D1%7D%5En%7BW_i%7D%5D](https://api.bilibili.com/x/web-frontend/mathjax/tex?formula=%5B1%2C%5Csum_%5Cnolimits%7Bi%3D1%7D%5En%7BW_i%7D%5D) 上均匀分布的整数 ![R](https://api.bilibili.com/x/web-frontend/mathjax/tex?formula=R) ， ![R](https://api.bilibili.com/x/web-frontend/mathjax/tex?formula=R) 落在区间 ![%5B%5Csum_%5Cnolimits%7Bi%3D1%7D%5E%7Bj-1%7D%7BW_i%7D%2B1%2C%5Csum_%5Cnolimits%7Bi%3D1%7D%5E%7Bj%7D%7BW_i%7D%5D](https://api.bilibili.com/x/web-frontend/mathjax/tex?formula=%5B%5Csum_%5Cnolimits%7Bi%3D1%7D%5E%7Bj-1%7D%7BW_i%7D%2B1%2C%5Csum_%5Cnolimits%7Bi%3D1%7D%5E%7Bj%7D%7BW_i%7D%5D) 即是选中了物品 ![A_j](https://api.bilibili.com/x/web-frontend/mathjax/tex?formula=A_j) 。

##### 适用于抽卡的轮盘选择法

如果完全根据权重来进行选择，实现让玩家必得某类物品的功能会比较繁琐。因此对朴素的轮盘选择法进行一点改变，设置权重和的上限 ![W_%7Bceil%7D](https://api.bilibili.com/x/web-frontend/mathjax/tex?formula=W_%7Bceil%7D) ，选取的随机数落在 ![%5B1%2Cmin(%5Csum_%5Cnolimits%7Bi%3D1%7D%5En%7BW_i%7D%2CW_%7Bceil%7D)%5D](https://api.bilibili.com/x/web-frontend/mathjax/tex?formula=%5B1%2Cmin(%5Csum_%5Cnolimits%7Bi%3D1%7D%5En%7BW_i%7D%2CW_%7Bceil%7D)%5D) ，并对物品按照从高优先级到低优先级的顺序编号。这样修改后，想要保证玩家必得某类物品时只需将其优先级设置为最高，并将其权重设置超过 ![W_%7Bceil%7D](https://api.bilibili.com/x/web-frontend/mathjax/tex?formula=W_%7Bceil%7D) ，即可简洁的实现必得某类物品。

**例子：**
>你是蒙德百货的售货员布兰琪小姐，接百货经理要求组织特卖活动鼓励旅行者来蒙德百货购物，以在与荣发商铺的竞争中取得优势。现在你想以购物满额度获得抽奖机会的方式来吸引旅行者。
> 
> 你认为购物每满一万摩拉获得一次抽奖机会比较合适，每次抽奖有10%概率获得大奖：蒙德百货一万摩拉代金券，有30%概率获得幸运奖：能治疗脱发的药，有60%的概率获得安慰奖：能用来盖房子的无相之雷方块。
> 
> 由于售货处正好有一个二十面骰子，你决定使用轮盘选择法进行抽奖，大奖权重2，幸运奖权重6，安慰奖权重12。骰子落到1-2时旅行者获得大奖，骰子落到3-8时旅行者获得幸运奖，骰子落到9-20时旅行者获得安慰奖。
> 
> 由于担心旅行者由于运气太背总是抽不到大奖，愤而前往荣发商铺购物，你贴心的设计了若连续9次抽奖都没有抽到大奖，下次抽奖大奖的权重变为20，这样骰子落到1-20时获得大奖，21-26时获得幸运奖，27-38时获得安慰奖。因为骰子产生的随机数上限为20，不可能落到21-38，此时旅行者必能够抽到大奖。
> 
> ![img](https://i0.hdslb.com/bfs/article/aea65cbbb15f358539845c0f252f1cfcfffc13ae.png@942w_159h_progressive.webp) 布兰琪小姐设计的抽奖系统
> 
> 现在你非常自信，使用这套抽奖规则开始了蒙德百货今日营业。旅行者已经来了，快向他推销吧！
> 
> ![img](https://i0.hdslb.com/bfs/article/016838ed75ac3c481d68ba0894a0bf3a790d6282.png@942w_629h_progressive.webp) 注意旅行者的头发

为方便讲解模型，本文之后的轮盘选均为![W_%7Bceil%7D%3D10000](https://api.bilibili.com/x/web-frontend/mathjax/tex?formula=W_%7Bceil%7D%3D10000) 的此类轮盘选择法。

### 决定抽到物品等级的机制

#### “保底”机制

每抽卡一次进行一次轮盘选以确定抽到什么等级的物品，常驻祈愿和角色活动祈愿各类物品的权重如下：

![%5Cbegin%7Barray%7D%7Bl%7D%0A%20%20%20%20W_%7B%E5%B8%B8%E9%A9%BB%E5%92%8C%E8%A7%92%E8%89%B2%E7%A5%88%E6%84%BF%E4%BA%94%E6%98%9F%7D%5Bi%5D%3D%0A%20%20%20%20%5Cleft%5C%7B%0A%20%20%20%20%20%20%20%20%20%20%20%20%5Cbegin%7Barray%7D%7Bl%7D%0A%20%20%20%20%20%20%20%20%20%20%20%2060%20%26%20(i%5Cleq73)%5C%5C%0A%20%20%20%20%20%20%20%20%20%20%20%2060%2B600%5Ccdot(i-73)%20%26%20(i%5Cgeq74)%0A%20%20%20%20%20%20%20%20%20%20%20%20%5Cend%7Barray%7D%0A%20%20%20%20%5Cright.%5C%5C%0A%20%20%20%20W_%7B%E5%B8%B8%E9%A9%BB%E5%92%8C%E8%A7%92%E8%89%B2%E7%A5%88%E6%84%BF%E5%9B%9B%E6%98%9F%7D%5Bj%5D%3D%0A%20%20%20%20%5Cleft%5C%7B%0A%20%20%20%20%20%20%20%20%20%20%20%20%5Cbegin%7Barray%7D%7Bl%7D%0A%20%20%20%20%20%20%20%20%20%20%20%20510%20%26%20%5Chfill(j%5Cleq8)%5C%5C%0A%20%20%20%20%20%20%20%20%20%20%20%20510%2B5100%5Ccdot(j-8)%20%26%20%5Chfill(j%5Cgeq9)%5C%5C%0A%20%20%20%20%20%20%20%20%20%20%20%20%5Cend%7Barray%7D%0A%20%20%20%20%5Cright.%5C%5C%0A%20%20%20%20W_%7B%E4%B8%89%E6%98%9F%E7%89%A9%E5%93%81%7D%3D9430%0A%5Cend%7Barray%7D](https://api.bilibili.com/x/web-frontend/mathjax/tex?formula=%5Cbegin%7Barray%7D%7Bl%7D%0A%20%20%20%20W_%7B%E5%B8%B8%E9%A9%BB%E5%92%8C%E8%A7%92%E8%89%B2%E7%A5%88%E6%84%BF%E4%BA%94%E6%98%9F%7D%5Bi%5D%3D%0A%20%20%20%20%5Cleft%5C%7B%0A%20%20%20%20%20%20%20%20%20%20%20%20%5Cbegin%7Barray%7D%7Bl%7D%0A%20%20%20%20%20%20%20%20%20%20%20%2060%20%26%20(i%5Cleq73)%5C%5C%0A%20%20%20%20%20%20%20%20%20%20%20%2060%2B600%5Ccdot(i-73)%20%26%20(i%5Cgeq74)%0A%20%20%20%20%20%20%20%20%20%20%20%20%5Cend%7Barray%7D%0A%20%20%20%20%5Cright.%5C%5C%0A%20%20%20%20W_%7B%E5%B8%B8%E9%A9%BB%E5%92%8C%E8%A7%92%E8%89%B2%E7%A5%88%E6%84%BF%E5%9B%9B%E6%98%9F%7D%5Bj%5D%3D%0A%20%20%20%20%5Cleft%5C%7B%0A%20%20%20%20%20%20%20%20%20%20%20%20%5Cbegin%7Barray%7D%7Bl%7D%0A%20%20%20%20%20%20%20%20%20%20%20%20510%20%26%20%5Chfill(j%5Cleq8)%5C%5C%0A%20%20%20%20%20%20%20%20%20%20%20%20510%2B5100%5Ccdot(j-8)%20%26%20%5Chfill(j%5Cgeq9)%5C%5C%0A%20%20%20%20%20%20%20%20%20%20%20%20%5Cend%7Barray%7D%0A%20%20%20%20%5Cright.%5C%5C%0A%20%20%20%20W_%7B%E4%B8%89%E6%98%9F%E7%89%A9%E5%93%81%7D%3D9430%0A%5Cend%7Barray%7D)

武器活动祈愿各类物品的权重如下：

![%5Cbegin%7Barray%7D%7Bl%7D%0A%20%20%20%20W_%7B%E6%AD%A6%E5%99%A8%E7%A5%88%E6%84%BF%E4%BA%94%E6%98%9F%7D%5Bi%5D%3D%0A%20%20%20%20%5Cleft%5C%7B%0A%20%20%20%20%20%20%20%20%20%20%20%20%5Cbegin%7Barray%7D%7Bl%7D%0A%20%20%20%20%20%20%20%20%20%20%20%2070%20%26%20%5Chfill(i%5Cleq62)%5C%5C%0A%20%20%20%20%20%20%20%20%20%20%20%2070%2B700%5Ccdot(i-62)%20%26%20%5Chfill(63%5Cleq%20i%5Cleq73)%5C%5C%0A%20%20%20%20%20%20%20%20%20%20%20%207770%2B350%5Ccdot(i-73)%20%26%20%5Chfill(i%5Cgeq74)%0A%20%20%20%20%20%20%20%20%20%20%20%20%5Cend%7Barray%7D%0A%20%20%20%20%5Cright.%5C%5C%0A%20%20%20%20W_%7B%E6%AD%A6%E5%99%A8%E7%A5%88%E6%84%BF%E5%9B%9B%E6%98%9F%7D%5Bj%5D%3D%0A%20%20%20%20%5Cleft%5C%7B%0A%20%20%20%20%20%20%20%20%20%20%20%20%5Cbegin%7Barray%7D%7Bl%7D%0A%20%20%20%20%20%20%20%20%20%20%20%20600%20%26%20%5Chfill(j%5Cleq7)%5C%5C%0A%20%20%20%20%20%20%20%20%20%20%20%206600%20%26%20%5Chfill(j%3D8)%5C%5C%0A%20%20%20%20%20%20%20%20%20%20%20%206600%2B3000%5Ccdot(j-8)%20%26%20%5Chfill(j%5Cgeq9)%5C%5C%0A%20%20%20%20%20%20%20%20%20%20%20%20%5Cend%7Barray%7D%0A%20%20%20%20%5Cright.%5C%5C%0A%20%20%20%20W_%7B%E4%B8%89%E6%98%9F%E7%89%A9%E5%93%81%7D%3D9330%0A%5Cend%7Barray%7D](https://api.bilibili.com/x/web-frontend/mathjax/tex?formula=%5Cbegin%7Barray%7D%7Bl%7D%0A%20%20%20%20W_%7B%E6%AD%A6%E5%99%A8%E7%A5%88%E6%84%BF%E4%BA%94%E6%98%9F%7D%5Bi%5D%3D%0A%20%20%20%20%5Cleft%5C%7B%0A%20%20%20%20%20%20%20%20%20%20%20%20%5Cbegin%7Barray%7D%7Bl%7D%0A%20%20%20%20%20%20%20%20%20%20%20%2070%20%26%20%5Chfill(i%5Cleq62)%5C%5C%0A%20%20%20%20%20%20%20%20%20%20%20%2070%2B700%5Ccdot(i-62)%20%26%20%5Chfill(63%5Cleq%20i%5Cleq73)%5C%5C%0A%20%20%20%20%20%20%20%20%20%20%20%207770%2B350%5Ccdot(i-73)%20%26%20%5Chfill(i%5Cgeq74)%0A%20%20%20%20%20%20%20%20%20%20%20%20%5Cend%7Barray%7D%0A%20%20%20%20%5Cright.%5C%5C%0A%20%20%20%20W_%7B%E6%AD%A6%E5%99%A8%E7%A5%88%E6%84%BF%E5%9B%9B%E6%98%9F%7D%5Bj%5D%3D%0A%20%20%20%20%5Cleft%5C%7B%0A%20%20%20%20%20%20%20%20%20%20%20%20%5Cbegin%7Barray%7D%7Bl%7D%0A%20%20%20%20%20%20%20%20%20%20%20%20600%20%26%20%5Chfill(j%5Cleq7)%5C%5C%0A%20%20%20%20%20%20%20%20%20%20%20%206600%20%26%20%5Chfill(j%3D8)%5C%5C%0A%20%20%20%20%20%20%20%20%20%20%20%206600%2B3000%5Ccdot(j-8)%20%26%20%5Chfill(j%5Cgeq9)%5C%5C%0A%20%20%20%20%20%20%20%20%20%20%20%20%5Cend%7Barray%7D%0A%20%20%20%20%5Cright.%5C%5C%0A%20%20%20%20W_%7B%E4%B8%89%E6%98%9F%E7%89%A9%E5%93%81%7D%3D9330%0A%5Cend%7Barray%7D)

模型中有两个计数器 ![i%2Cj](https://api.bilibili.com/x/web-frontend/mathjax/tex?formula=i%2Cj) ，其中![i](https://api.bilibili.com/x/web-frontend/mathjax/tex?formula=i) 表示此前已经连续 ![i-1](https://api.bilibili.com/x/web-frontend/mathjax/tex?formula=i-1) 抽没有抽到五星物品， ![j](https://api.bilibili.com/x/web-frontend/mathjax/tex?formula=j) 表示此前已经连续 ![j-1](https://api.bilibili.com/x/web-frontend/mathjax/tex?formula=j-1) 抽没有抽到四星物品。需要注意，每种类型的祈愿都有独立的计数器，且四星五星计数器独立。保底机制中，五星优先于四星，四星优先于三星。或许是为了程序实现方便，四星和五星物品的实际综合概率均略高于公示概率。

### 决定抽到物品类别的机制

#### “概率UP”机制

角色活动祈愿、武器活动祈愿中有“概率UP”机制。在确定抽取到的物品星级后，会按照UP规则划分抽取物品类别。

角色活动祈愿UP规则为：若上次抽到的五星为非UP五星，本次必定抽到UP五星。若上次抽到的五星是UP五星，本次有50%的概率抽到UP五星。

武器活动祈愿UP规则为：若上次抽到的五星为非UP五星，本次必定抽到UP五星。若上次抽到的五星是UP五星，本次有75%的概率抽到UP五星。

抽取到UP类型物品时，UP类型物品中各物品均分概率。当未触发UP机制时，抽到的类型内各项物品均分概率，注意这其中也包含UP物品。注意，当UP池轮换时，保底计数器和上次抽到的五星是否为UP五星的记录不重置。

#### “平稳”机制

常驻祈愿中四星及五星物品、角色活动祈愿中四星物品、武器活动祈愿中的四星物品有“平稳”机制。在确定抽取到的物品星级并未触发UP机制时，会按照平稳机制确定抽到物品是角色还是武器。当很长时间没有抽到角色时，下次抽取更可能获得角色。当很长时间没有抽到武器时，下次抽取更可能获得武器。这样的机制可以避免玩家有角色却没有武器，或有武器却没有角色的情况出现。

启用平稳机制时，进行一次轮盘选以确定抽到什么类别的物品，武器和角色各有权重，权重如下：

常驻祈愿五星：

![W_%7B%E5%B8%B8%E9%A9%BB%E7%A5%88%E6%84%BF%E4%BA%94%E6%98%9F%7D%5Bi%5D%3D%0A%5Cleft%5C%7B%0A%20%20%20%20%5Cbegin%7Barray%7D%7Bl%7D%0A%20%20%20%2030%20%26%20%5Chfill(i%5Cleq147)%5C%5C%0A%20%20%20%2030%2B300%5Ccdot(i-147)%20%26%20%5Chfill(i%5Cgeq148)%5C%5C%0A%20%20%20%20%5Cend%7Barray%7D%0A%5Cright.%5C%5C](https://api.bilibili.com/x/web-frontend/mathjax/tex?formula=W_%7B%E5%B8%B8%E9%A9%BB%E7%A5%88%E6%84%BF%E4%BA%94%E6%98%9F%7D%5Bi%5D%3D%0A%5Cleft%5C%7B%0A%20%20%20%20%5Cbegin%7Barray%7D%7Bl%7D%0A%20%20%20%2030%20%26%20%5Chfill(i%5Cleq147)%5C%5C%0A%20%20%20%2030%2B300%5Ccdot(i-147)%20%26%20%5Chfill(i%5Cgeq148)%5C%5C%0A%20%20%20%20%5Cend%7Barray%7D%0A%5Cright.%5C%5C)

常驻及角色活动祈愿四星：

![W_%7B%E5%B8%B8%E9%A9%BB%E5%92%8C%E8%A7%92%E8%89%B2%E7%A5%88%E6%84%BF%E5%9B%9B%E6%98%9F%7D%5Bj%5D%3D%0A%5Cleft%5C%7B%0A%20%20%20%20%5Cbegin%7Barray%7D%7Bl%7D%0A%20%20%20%20255%20%26%20(j%5Cleq17)%5C%5C%0A%20%20%20%20255%2B2550%5Ccdot(j-17)%20%26%20(j%5Cgeq18)%0A%20%20%20%20%5Cend%7Barray%7D%0A%5Cright.%5C%5C](https://api.bilibili.com/x/web-frontend/mathjax/tex?formula=W_%7B%E5%B8%B8%E9%A9%BB%E5%92%8C%E8%A7%92%E8%89%B2%E7%A5%88%E6%84%BF%E5%9B%9B%E6%98%9F%7D%5Bj%5D%3D%0A%5Cleft%5C%7B%0A%20%20%20%20%5Cbegin%7Barray%7D%7Bl%7D%0A%20%20%20%20255%20%26%20(j%5Cleq17)%5C%5C%0A%20%20%20%20255%2B2550%5Ccdot(j-17)%20%26%20(j%5Cgeq18)%0A%20%20%20%20%5Cend%7Barray%7D%0A%5Cright.%5C%5C)

武器活动祈愿四星：

![W_%7B%E6%AD%A6%E5%99%A8%E7%A5%88%E6%84%BF%E5%9B%9B%E6%98%9F%7D%5Bj%5D%3D%0A%5Cleft%5C%7B%0A%20%20%20%20%5Cbegin%7Barray%7D%7Bl%7D%0A%20%20%20%20300%20%26%20(j%5Cleq15)%5C%5C%0A%20%20%20%20300%2B3000%5Ccdot(j-15)%20%26%20(j%5Cgeq16)%0A%20%20%20%20%5Cend%7Barray%7D%0A%5Cright.%5C%5C](https://api.bilibili.com/x/web-frontend/mathjax/tex?formula=W_%7B%E6%AD%A6%E5%99%A8%E7%A5%88%E6%84%BF%E5%9B%9B%E6%98%9F%7D%5Bj%5D%3D%0A%5Cleft%5C%7B%0A%20%20%20%20%5Cbegin%7Barray%7D%7Bl%7D%0A%20%20%20%20300%20%26%20(j%5Cleq15)%5C%5C%0A%20%20%20%20300%2B3000%5Ccdot(j-15)%20%26%20(j%5Cgeq16)%0A%20%20%20%20%5Cend%7Barray%7D%0A%5Cright.%5C%5C)

权重表达式中的参数 ![x](https://api.bilibili.com/x/web-frontend/mathjax/tex?formula=x) 表示此前已经连续![x-1](https://api.bilibili.com/x/web-frontend/mathjax/tex?formula=x-1) 抽没有抽到本类物品。如常驻祈愿中已有148抽没有抽到五星角色，74抽没有抽到五星武器时抽卡一次抽到了五星，有 ![W_%7B%E8%A7%92%E8%89%B2%7D%3D630%2CW_%7B%E6%AD%A6%E5%99%A8%7D%3D30](https://api.bilibili.com/x/web-frontend/mathjax/tex?formula=W_%7B%E8%A7%92%E8%89%B2%7D%3D630%2CW_%7B%E6%AD%A6%E5%99%A8%7D%3D30) 。“平稳”机制中优先级按照权重排序，权重最大的优先级最高。

</details>

## TODO

- [ ] Python版本（因为Python iterate太慢了，除了矩阵运算外暂时找不到好的方式，希望有矩阵贼6的大佬加入）
- [ ] GUI (Python版本预计会使用 [Gooey](https://github.com/chriskiehl/Gooey)，C++不会)

## Credits

[抽卡模型的研究](https://www.bilibili.com/read/cv10468091) ： [一棵平衡树](https://space.bilibili.com/6165300)

[C++及Excel版本计算器](https://bbs.nga.cn/read.php?tid=26447862) ： [孤胆飞客](https://bbs.nga.cn/nuke.php?func=ucp&uid=43116386)

