/*

# k6 Load Testing Script for Auction System
k6を使用したオークションシステムの負荷テストスクリプト

使い方:
step1:  

 動かす前にk6のインストール
 brew install k6  
 をしてください

step2:
 高速化したauctionシステムを make run で起動しておく

step3: 
 以下のコマンドで負荷テストを実行する。
 負荷テストをしている間はgrafanaダッシュボートで見ると良い

  # 基本的なテスト実行
  k6 run auction-load-test.js

  # 結果をJSONで出力
  k6 run --out json=results.json auction-load-test.js

  # 特定のステージのみ実行
  k6 run --stage 1m:10,2m:20,1m:0 auction-load-test.js

step4:
  結果の確認
    k6 stats results.json


*/
import http from 'k6/http';
import { check } from 'k6';

export const options = {
  scenarios: {
    // 定常負荷テスト
    constant_load: {
      executor: 'constant-vus',
      vus: 100,
      duration: '1m',
    },
    // スパイクテスト
    spike_test: {
      executor: 'ramping-vus',
      startTime: '1m',
      stages: [
        { duration: '10s', target: 500 },
        { duration: '30s', target: 500 },
        { duration: '10s', target: 0 },
      ],
    },
    // 段階的負荷増加
    stress_test: {
      executor: 'ramping-vus',
      startTime: '6m',
      stages: [
        { duration: '1m', target: 100 },
        { duration: '1m', target: 200 },
        { duration: '1m', target: 300 },
        { duration: '1m', target: 400 },
        { duration: '1m', target: 500 },
        { duration: '2m', target: 0 },
      ],
    },
  },
  thresholds: {
    http_req_duration: ['p(99)<1000'],
    http_req_failed: ['rate<0.05'],
  },
};

export default function () {
  const response = http.post('http://localhost:6562/auction', 
    JSON.stringify({
      "auction_id": "xxxxxxxxxxx",
    "category_ids": [
        1,
        3,
        5,
        8,
        14
    ],
    "zone_feature": 31415,
    "zone_width": 200,
    "zone_height": 300,
    "block_domains": [
        "example",
        "test"
    ]
    }), 
    { headers: { 'Content-Type': 'application/json' } }
  );

  check(response, {
    'status is 200 or 204': (r) => r.status === 200 || r.status === 204,
    'response time < 500ms': (r) => r.timings.duration < 500,
  });
}
