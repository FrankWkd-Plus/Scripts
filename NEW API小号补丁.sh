#!/bin/bash

# ============================================
# New API - 密码重置工具（已配置好参数）
# ============================================

# ===== 配置区（已填好）=====
BASE_URL="https://中转站域名.com"
SYSTEM_TOKEN="flK034Svi4+KVDAgJdnaD16psI3u"  # 你的 Token
USER_ID="118"  # 你的用户 ID
NEW_PASSWORD="Frank1202"  # 请修改这个
# ================================

# 颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

info_log() { echo -e "${GREEN}[INFO]${NC} $1"; }
error_log() { echo -e "${RED}[ERROR]${NC} $1"; }

info_log "开始重置密码..."
info_log "用户 ID: ${USER_ID}"
info_log "用户: FrankWkd"

# 调用重置密码接口
response=$(curl -s -w "\n%{http_code}" -X POST "${BASE_URL}/api/user/reset" \
    -H "Authorization: Bearer ${SYSTEM_TOKEN}" \
    -H "New-Api-User: ${USER_ID}" \
    -H "Content-Type: application/json" \
    -d "{\"password\":\"${NEW_PASSWORD}\"}" 2>&1)

http_code=$(echo "$response" | tail -n1)
body=$(echo "$response" | sed '$d')

echo ""
if [ $http_code -eq 200 ]; then
    info_log "✅ 密码重置成功！"
    info_log "新密码: ${NEW_PASSWORD}"
    info_log ""
    info_log "现在可以访问 ${BASE_URL} 使用以下方式登录："
    info_log "  - 用户名: FrankWkd"
    info_log "  - 密码: ${NEW_PASSWORD}"
else
    error_log "密码重置失败（HTTP ${http_code}）"
    error_log "响应: ${body}"
fi
