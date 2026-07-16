#!/bin/bash

# 配置信息
API_URL="https://你的域名/api/public/emailList"
TOKEN="768b1907-d6bd-4be6-ad48-967878da7bb8"

# 发送请求并处理结果
# 逻辑：获取 data 数组的第 0 个元素，并美化输出
response=$(curl -s -POST "$API_URL" \
     -H "Authorization: $TOKEN" \
     -H "Content-Type: application/json" \
     -d '{"num": 1, "size": 1}')

# 检查是否请求成功
code=$(echo "$response" | jq '.code')

if [ "$code" == "200" ]; then
    echo "------------------------------------------"
    echo "最新邮件内容如下："
    echo "------------------------------------------"
    # 提取发送人、主题和正文文本
    echo "$response" | jq -r '.data[0] | "发件人: \(.sendName) <\(.sendEmail)>\n主题: \(.subject)\n时间: \(.createTime)\n\n内容概要:\n\(.text)"'
else
    echo "请求失败，错误代码: $code"
    echo "完整响应: $response"
fi
